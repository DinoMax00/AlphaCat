#include <iostream>
#include <Windows.h>

#include "ucci.h"
#include "move.h"
#include "hash.h"
#include "search.h"

SearchStruct Search;

uint16_t opBookSearch(uint64_t obj, int mirror);

// 无害剪裁
inline int harmlessPruning(int beta)
{
	int val;

	// 杀棋步数剪裁
	val = Search.pos.depth - MATE_VALUE;
	if (val >= beta)
		return val;

	/*
	// 和棋剪裁
	if (Search.pos.isDraw())
		return 0;
	*/

	// 重复剪裁
#ifdef CHASE_CHECK
	int val_rep = Search.pos.detectCircle2();
#else
	int val_rep = Search.pos.detectCircle();
#endif // CHASE_CHECK

	if (val_rep > 0)
		return Search.pos.circleVal(val_rep);

	return -MATE_VALUE;
}

// 静态搜索
int quieseSearch(int alpha, int beta)
{
	Search.nodes++;
	MoveSort move_sort;
	int  val;

	// 无害剪裁
	val = harmlessPruning(beta);

	if (val > -MATE_VALUE)
	{
		return val;
	}

	// 极限深度返回局面估值
	if (Search.pos.depth >= LIMIT_DEPTH)
		return Search.pos.getValue(alpha, beta);

	// 初始化
	int best = -MATE_VALUE;

	bool check = Search.pos.lastMove().ChkChs > 0;
	// 将军检测
	if (check)
	{
		move_sort.getQuiescAll(Search.pos);
	}
	else
	{
		val = Search.pos.getValue(alpha, beta);
		if (val >= beta)
			return val;
		best = val;
		if (val > alpha)
			alpha = val;
		// 生成吃子着法
		move_sort.getCapMoves(Search.pos);
	}

	// 向下搜索
	int16_t mv;
	while (mv = move_sort.nextQuiesc(check))
	{
		if (Search.pos.takeOneMove(mv))
		{
			val = -quieseSearch(-beta, -alpha);
			Search.pos.deleteOneMove();
			if (val > best)
			{
				if (val >= beta)
					return val;
				best = val;
				if (val > alpha)
					alpha = val;
			}
		}
	}

	// 返回分值
	if (best == -MATE_VALUE)
	{
		return Search.pos.depth - MATE_VALUE;
	}
	return best;
}

// 主要变例搜索
int pvSearch(int depth, int alpha, int beta, bool no_null = false)
{
	Search.nodes++;
	int val;
	bool flagPV = false;//主要变例搜索
	uint16_t best_move = 0;

	if (depth <= 0)
	{
		// 对于叶子节点，进行静态搜索
		// return Search.pos.getValue(alpha, beta);
		return quieseSearch(alpha, beta);
	}

	// 无害剪裁
	val = harmlessPruning(beta);
	if (val > -MATE_VALUE)
		return val;

	// 置换剪裁
	int hashflag = FLAG_ALPHA;
	int vlhash, mvhash = 0;
	vlhash = getHashTable(Search.pos, alpha, beta, depth, true, mvhash); 

	if (vlhash > -MATE_VALUE) {
		return vlhash;
	}

	// 极限深度 返回估值
	if (Search.pos.depth >= LIMIT_DEPTH)
		return Search.pos.getValue(alpha, beta);

	// 限时
	if (GetTickCount64() - Search.cur_time > Search.time_limit)
	{
		Search.stop = true;
		return Search.pos.getValue(alpha, beta);
	}

	// 空着剪裁
	if (!no_null && Search.pos.lastMove().ChkChs <= 0 && Search.pos.nullOk())
	{
		Search.pos.nullMove();
		val = -pvSearch(depth - NULL_DEPTH - 1, -beta, -beta + 1, true);
		Search.pos.deleteNullMove();

		if (val >= beta)
		{
			if (Search.pos.nullSafe())
				return val;
			if (pvSearch(depth - NULL_DEPTH, alpha, beta, true) >= beta)
				return val;
		}
	}

	int best = -MATE_VALUE;

	// 着法生成
	MoveSort move_sort;
	move_sort.getAllMoves(Search.pos, mvhash, Search.killeTable[Search.pos.depth]);
	int mv, new_depth;

	while (mv = move_sort.next(Search.pos))
	{
		if (!Search.pos.takeOneMove(mv))
			continue;

		// 尝试选择延伸
		new_depth = Search.pos.lastMove().ChkChs > 0 ? depth : depth - 1;
		if (best == -MATE_VALUE)
			val = -pvSearch(new_depth, -beta, -alpha);
		else
		{
			val = -pvSearch(new_depth, -alpha - 1, -alpha);
			if (val > alpha && val < beta)
				val = -pvSearch(new_depth, -beta, -alpha);
		}

		Search.pos.deleteOneMove();

		// 限时
		if (Search.stop)
			return best;

		// alpha_beta边界判定
		if (val > best)
		{
			best = val;
			if (val >= beta)
			{
				best_move = mv;
				hashflag = FLAG_BETA;
				break;
			}
			if (val > alpha)
			{
				best_move = mv;
				hashflag = FLAG_PV;
				alpha = val;
			}
		}
	}

	if (best == -MATE_VALUE)
		return Search.pos.depth - MATE_VALUE;
	else
	{
		// 更新置换表
		recordHashTable(Search.pos.zobrist, hashflag, best, depth, mv);
		if (best_move)
		{
			updBest(best_move, depth, Search.killeTable[Search.pos.depth]);
		}
		return best;
	}
}

// 根节点搜索
int searchRoot(int depth)
{
	Search.nodes++;
	MoveSort move_sort;
	move_sort.getAllMoves(Search.pos, 0);
	int16_t mv;
	int new_depth, val;
	int best = -MATE_VALUE;

	// 搜索根节点下着法
	while (mv = move_sort.next(Search.pos))
	{
		if (!Search.pos.takeOneMove(mv))
			continue;

		// 尝试性延伸
		new_depth = Search.pos.lastMove().ChkChs > 0 ? depth : depth - 1;

		// 主要变例搜索
		if (best == -MATE_VALUE)
			val = -pvSearch(new_depth, -MATE_VALUE, MATE_VALUE);
		else
		{
			val = -pvSearch(new_depth, -best - 1, -best);
			if (val > best)
				val = -pvSearch(new_depth, -MATE_VALUE, -best);
		}

		Search.pos.deleteOneMove();

		// 更新最优着法
		if (val > best)
		{
			best = val;
			Search.result = mv;
		}

		if (GetTickCount64() - Search.cur_time > Search.time_limit)
		{
			Search.stop = true;
		}
	}

	updBest(Search.result, depth, Search.killeTable[Search.pos.depth]);

	return best;
}

void initSearch()
{
	Search.nodes = 0;
	Search.pos.depth = 0;
	Search.result = -1;
	Search.time_limit = 5000;
	Search.stop = false;
	clearHashTable();
	clearHistory();
	Search.clearKiller();
}

void searchMain(int depth)
{
	int val;
	int16_t best_move = -1;

	// 初始化
	initSearch();

	// 开局库
	/*
	int opbook_result = opBookSearch(Search.pos.zobrist, 0);
	if (opbook_result != 0)
	{
		Search.result = opbook_result;
		return;
	}
	opbook_result = opBookSearch(Search.pos.zobrist_mirror, 1);
	if (opbook_result != 0)
	{
		Search.result = opbook_result;
		return;
	}
	*/
	// 开始计时
	Search.cur_time = GetTickCount64();

	// 迭代加深
	int i;
	for (i = 1; i <= depth; i++)
	{
		val = searchRoot(i);

		if (Search.stop)
		{
			break;
		}
		best_move = Search.result;

		// 搜到杀棋 终止搜索
		if (val > WIN_VALUE || val < -WIN_VALUE)
		{
			break;
		}
	}
	Search.result = best_move;

	std::cout << "搜索层数: " << i - 1 << std::endl;
	std::cout << "用时: " << GetTickCount64() - Search.cur_time << "ms" << std::endl;
	std::cout << "结点数量: " << Search.nodes << std::endl;
	return;
}


