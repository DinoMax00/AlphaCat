
#include "search.h"
#include "game.h"
#include "pregen.h"
#include "evaluate.h"
#include <sys/timeb.h>

const int IID_DEPTH = 2;       // 内部迭代加深的深度
const int SMP_DEPTH = 6;       // 并行搜索的深度
const int UNCHANGED_DEPTH = 4; // 未改变最佳着法的深度

const int DROPDOWN_VALUE = 20;   // 落后的分值
const int RESIGN_VALUE = 300;    // 认输的分值
const int DRAW_OFFER_VALUE = 40; // 提和的分值

SearchStruct Search;
// 搜索信息，是封装在模块内部的
static struct {
    int64_t llTime;                   // 计时器
    bool bStop, bPonderStop;          // 中止信号和后台思考认为的中止信号
    bool bPopPv, bPopCurrMove;        // 是否输出pv和currmove
    int nPopDepth, vlPopValue;        // 输出的深度和分值
    int nAllNodes, nMainNodes;        // 总结点数和主搜索树的结点数
    int nUnchanged;                   // 未改变最佳着法的深度
    MoveSortStruct MoveSort;          // 根结点的着法序列
    uint16_t wmvPvLine[MAX_MOVE_NUM]; // 主要变例路线上的着法列表
} searchData;

int searchAlphaBeta(int depth, int alpha, int beta) {

    if (depth == 0 || (popCnt32(Search.pos.bitPieces & bothBitpiece(JIANG_BITPIECE))<2)) {
        return Search.pos.getGameVal();
    }
    
    MoveSortStruct MoveSort;
    MoveSort.InitFull(Search.pos, Search2.wmvKiller[Search.pos.nDistance]);

    if (MoveSort.NOMOVE())
        return -1000000;

    for (int i = 0; i < size; i++) {

        now.genOneMove(moveInDep[depth][i]);
        val = -searchAlphaBeta(now, depth - 1, -beta, -alpha);
        now.deleteOneMove(moveInDep[depth][i]);

        if (val >= beta) {
            return beta;
        }

        if (val > alpha) {
            alpha = val;
            if (depth == max_deep)
                best = i;
        }
    }
    return alpha;
}

uint16_t SearchMain(int nDepth) {
    int i, vl, vlLast, nDraw;
    int nCurrTimer, nLimitTimer, nLimitNodes;
    bool bUnique;
    int nBookMoves;
    // 主搜索例程包括以下几个步骤：

    // 1. 遇到和棋则直接返回，暂无

    // 2. 从开局库中搜索着法，暂无

    // 3. 如果深度为零则返回静态搜索值，暂无

    // 4. 生成根结点的每个着法
    searchData.MoveSort.InitRoot(Search.pos, Search.nBanMoves, Search.wmvBanList);

    // 5. 初始化时间和计数器
    searchData.bStop = searchData.bPonderStop = searchData.bPopPv = searchData.bPopCurrMove = false;
    searchData.nPopDepth = searchData.vlPopValue = 0;
    searchData.nAllNodes = searchData.nMainNodes = searchData.nUnchanged = 0;
    searchData.wmvPvLine[0] = 0;
    // 由于 ClearHash() 需要消耗一定时间，所以计时从这以后开始比较合理
    searchData.llTime = GetTime();
    vlLast = 0;

    // 6. 做迭代加深搜索
    for (i = 1; i <= nDepth; i++) {

        // 7. 根据搜索的时间决定，是否需要输出主要变例和当前思考的着法
        searchData.bPopPv = (nCurrTimer > 300);
        searchData.bPopCurrMove = (nCurrTimer > 3000);

        // 8. 搜索根结点
        vl = SearchRoot(i);// 改成alpha-beta
        if (searchData.bStop) {
            if (vl > -MATE_VALUE) {
                vlLast = vl; // 跳出后，vlLast会用来判断认输或投降，所以需要给定最近一个值
            }
            break; // 没有跳出，则"vl"是可靠值
        }

        nCurrTimer = (int)(GetTime() - searchData.llTime);
        // 9. 如果搜索时间超过适当时限，则终止搜索
        // a. 如果没有使用空着裁剪，那么适当时限减半(因为分枝因子加倍了)
        // nLimitTimer = (Search.bNullMove ? Search.nProperTimer : Search.nProperTimer / 2);
        // b. 如果当前搜索值没有落后前一层很多，那么适当时限减半
        // nLimitTimer = (vl + DROPDOWN_VALUE >= vlLast ? nLimitTimer / 2 : nLimitTimer);
        // c. 如果最佳着法连续多层没有变化，那么适当时限减半
        // nLimitTimer = (searchData.nUnchanged >= UNCHANGED_DEPTH ? nLimitTimer / 2 : nLimitTimer);
        if (nCurrTimer > nLimitTimer) {
            if (Search.bPonder) {
                searchData.bPonderStop = true; // 如果处于后台思考模式，那么只是在后台思考命中后立即中止搜索。
            } else {
                vlLast = vl;
                break; // 不管是否跳出，"vlLast"都已更新
            }
        }
        vlLast = vl;

        // 10. 搜索到杀棋则终止搜索
        if (vlLast > WIN_VALUE || vlLast < -WIN_VALUE) {
            break;
        }

        // 11. 是唯一着法，则终止搜索, 暂无
    }

    Search.mvResult = searchData.wmvPvLine[0];
    // 12. 返回最佳着法
    if (searchData.wmvPvLine[0] != 0) {
        // PopPvLine();
        return searchData.wmvPvLine[0];
        // 13. 判断是否认输或提和， 无
    } else {
        return 0;
    }
}