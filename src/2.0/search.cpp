#include <iostream>
#include <Windows.h>

#include "ucci.h"
#include "move.h"
#include "hash.h"
#include "search.h"
#include "log.h"
SearchStruct Search;

// 无害剪裁
inline int harmlessPruning(int beta)
{
	int val;

	// 杀棋步数剪裁
	val = Search.pos.depth - MATE_VALUE;
	if (val >= beta)
		return val;

	// 循环局面剪裁

	// 重复剪裁
	int val_rep = Search.pos.detectCircle();
	if (val_rep > 0)
		return Search.pos.circleVal(val_rep);

	return -MATE_VALUE;
}

// 静态搜索
int quieseSearch(int alpha, int beta)
{
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

	// 将军检测
	if (Search.pos.lastMove().ChkChs > 0)
	{
		move_sort.getAllMoves(Search.pos);
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
	while (mv = move_sort.nextQuiesc())
	{
		if (Search.pos.takeOneMove(mv))
		{
			int val = -quieseSearch(-beta, -alpha);
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

// 零窗口完全搜索例程
int cutSearch(int depth, int beta, bool no_null = false)
{
	int val, mvhash;
	MoveSort move_sort;

	// 叶子节点处调用静态搜索
	if (depth <= 0)
	{
		return quieseSearch(beta - 1, beta);
	}

	// 无害剪裁
	val = harmlessPruning(beta);
	if (val > -MATE_VALUE)
		return val;

	// 置换剪裁
	val = getHashTable(Search.pos.zobrist, beta - 1, beta, depth, mvhash);
	if (val > -MATE_VALUE)
	{
		return val;
	}

	// 极限深度 直接返回评价值
	if (Search.pos.depth >= LIMIT_DEPTH)
	{
		return Search.pos.getValue(beta - 1, beta);
	}

	// 空着剪裁
	if (!no_null && Search.pos.lastMove().ChkChs <= 0 && Search.pos.nullOk())
	{

	}
} 

// 主要变例搜索
int pvSearch(int depth, int alpha, int beta)
{
	int val;
	bool flagPV = false;//主要变例搜索
	if (depth == 0)
	{
		//对于叶子节点，进行静态搜索
		return quieseSearch(alpha, beta);
	}

	// 无害剪裁
	val = harmlessPruning(beta);
	if (val > -MATE_VALUE)
		return val;

	// 置换剪裁
	int hashflag = FLAG_ALPHA;
	int vlhash, mvhash;
	vlhash = getHashTable(Search.pos.zobrist, alpha, beta, depth, mvhash);
	if (vlhash > -MATE_VALUE)
		return vlhash;

	// 极限深度 返回估值
	if (Search.pos.depth >= LIMIT_DEPTH)
		return Search.pos.getValue(alpha, beta);

	int best = -MATE_VALUE;

	//// 内部迭代加深启发
	//if (depth > 2)
	//{
	//	int val = pvSearch(Search.pos, depth / 2, alpha, beta, max_deep);
	//	if (val <= alpha)
	//		val = pvSearch(Search.pos, depth / 2, -MATE_VALUE, beta, max_deep);
	//}

	MoveSort move_sort;
	move_sort.getAllMoves(Search.pos);
	int mv, new_depth;
	while (mv = move_sort.next())
	{
		if (!Search.pos.takeOneMove(mv))
			continue;
		// 尝试选择延伸
		new_depth = Search.pos.detectCheck() ? depth : depth - 1;

		if (best == -MATE_VALUE)
			val = -pvSearch(new_depth, -beta, -alpha);
		else
		{
			val = -pvSearch(new_depth, -alpha - 1, -alpha);
			if (val > alpha && val < beta)
				val = -pvSearch(new_depth, -beta, -alpha);
		}

		Search.pos.deleteOneMove();

		// alpha_beta边界判定
		if (val > best)
		{
			best = val;
			if (val >= beta)
			{
				hashflag = FLAG_BETA;
				break;
			}
			if (val > alpha)
			{
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
		//mv随便传了一个，最后需改
		return best;
	}
}

// 根节点搜索
int searchRoot(int depth)
{
	MoveSort move_sort;
	move_sort.getAllMoves(Search.pos);
	int16_t mv;
	int new_depth, val;
	int best = -MATE_VALUE;

	// 搜索根节点下着法
	while (mv = move_sort.next())
	{
		if (!Search.pos.takeOneMove(mv))
			continue;
		
		// 尝试性延伸
		new_depth = Search.pos.detectCheck() ? depth : depth - 1;
		
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

		// 计时
		if (Search.stop)
		{
			return best;
		}

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
	return best;
}

void initSearch()
{
	Search.pos.depth = 0;
	Search.result = -1;
	Search.time_limit = 1500;
	Search.stop = false;
}

void searchMain(int depth)
{
	int val, val_last;
	int16_t best_move = -1;
	uint64_t time_temp;

	// 初始化
	initSearch();
	// 开局库

	// 初始化计数器

	// 清空哈希表 历史表 杀手着法表
	clearHashTable();
	clearHistory();

	// 开始计时
	Search.cur_time = GetTickCount64();

	// 迭代加深
	for (int i = 1; i <= depth; i++)
	{
		val = searchRoot(i);

		if (Search.stop)
		{
			std::cout << "搜索层数: " << i << std::endl;
			Log().info(i);
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
	return;
}