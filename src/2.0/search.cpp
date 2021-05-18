
#include "search.h"
#include "game.h"
#include "pregen.h"
#include "evaluate.h"
#include <sys/timeb.h>

const int IID_DEPTH = 2;       // �ڲ�������������
const int SMP_DEPTH = 6;       // �������������
const int UNCHANGED_DEPTH = 4; // δ�ı�����ŷ������

const int DROPDOWN_VALUE = 20;   // ���ķ�ֵ
const int RESIGN_VALUE = 300;    // ����ķ�ֵ
const int DRAW_OFFER_VALUE = 40; // ��͵ķ�ֵ

SearchStruct Search;
// ������Ϣ���Ƿ�װ��ģ���ڲ���
static struct {
    int64_t llTime;                   // ��ʱ��
    bool bStop, bPonderStop;          // ��ֹ�źźͺ�̨˼����Ϊ����ֹ�ź�
    bool bPopPv, bPopCurrMove;        // �Ƿ����pv��currmove
    int nPopDepth, vlPopValue;        // �������Ⱥͷ�ֵ
    int nAllNodes, nMainNodes;        // �ܽ���������������Ľ����
    int nUnchanged;                   // δ�ı�����ŷ������
    MoveSortStruct MoveSort;          // �������ŷ�����
    uint16_t wmvPvLine[MAX_MOVE_NUM]; // ��Ҫ����·���ϵ��ŷ��б�
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
    // ���������̰������¼������裺

    // 1. ����������ֱ�ӷ��أ�����

    // 2. �ӿ��ֿ��������ŷ�������

    // 3. ������Ϊ���򷵻ؾ�̬����ֵ������

    // 4. ���ɸ�����ÿ���ŷ�
    searchData.MoveSort.InitRoot(Search.pos, Search.nBanMoves, Search.wmvBanList);

    // 5. ��ʼ��ʱ��ͼ�����
    searchData.bStop = searchData.bPonderStop = searchData.bPopPv = searchData.bPopCurrMove = false;
    searchData.nPopDepth = searchData.vlPopValue = 0;
    searchData.nAllNodes = searchData.nMainNodes = searchData.nUnchanged = 0;
    searchData.wmvPvLine[0] = 0;
    // ���� ClearHash() ��Ҫ����һ��ʱ�䣬���Լ�ʱ�����Ժ�ʼ�ȽϺ���
    searchData.llTime = GetTime();
    vlLast = 0;

    // 6. ��������������
    for (i = 1; i <= nDepth; i++) {

        // 7. ����������ʱ��������Ƿ���Ҫ�����Ҫ�����͵�ǰ˼�����ŷ�
        searchData.bPopPv = (nCurrTimer > 300);
        searchData.bPopCurrMove = (nCurrTimer > 3000);

        // 8. ���������
        vl = SearchRoot(i);// �ĳ�alpha-beta
        if (searchData.bStop) {
            if (vl > -MATE_VALUE) {
                vlLast = vl; // ������vlLast�������ж������Ͷ����������Ҫ�������һ��ֵ
            }
            break; // û����������"vl"�ǿɿ�ֵ
        }

        nCurrTimer = (int)(GetTime() - searchData.llTime);
        // 9. �������ʱ�䳬���ʵ�ʱ�ޣ�����ֹ����
        // a. ���û��ʹ�ÿ��Ųü�����ô�ʵ�ʱ�޼���(��Ϊ��֦���Ӽӱ���)
        // nLimitTimer = (Search.bNullMove ? Search.nProperTimer : Search.nProperTimer / 2);
        // b. �����ǰ����ֵû�����ǰһ��ܶ࣬��ô�ʵ�ʱ�޼���
        // nLimitTimer = (vl + DROPDOWN_VALUE >= vlLast ? nLimitTimer / 2 : nLimitTimer);
        // c. �������ŷ��������û�б仯����ô�ʵ�ʱ�޼���
        // nLimitTimer = (searchData.nUnchanged >= UNCHANGED_DEPTH ? nLimitTimer / 2 : nLimitTimer);
        if (nCurrTimer > nLimitTimer) {
            if (Search.bPonder) {
                searchData.bPonderStop = true; // ������ں�̨˼��ģʽ����ôֻ���ں�̨˼�����к�������ֹ������
            } else {
                vlLast = vl;
                break; // �����Ƿ�������"vlLast"���Ѹ���
            }
        }
        vlLast = vl;

        // 10. ������ɱ������ֹ����
        if (vlLast > WIN_VALUE || vlLast < -WIN_VALUE) {
            break;
        }

        // 11. ��Ψһ�ŷ�������ֹ����, ����
    }

    Search.mvResult = searchData.wmvPvLine[0];
    // 12. ��������ŷ�
    if (searchData.wmvPvLine[0] != 0) {
        // PopPvLine();
        return searchData.wmvPvLine[0];
        // 13. �ж��Ƿ��������ͣ� ��
    } else {
        return 0;
    }
}