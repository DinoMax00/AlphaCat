#include <iostream>

#include "ucci.h"
#include "move.h"
#include "search.h"

SearchStruct Search;

// 静态搜索
int quieseSearch(Game& game, int alpha, int beta)
{
	MoveSort move_sort;

	// 无害剪裁

	// 极限深度返回局面估值
	if (game.depth >= LIMIT_DEPTH)
		return game.getValue(alpha, beta);

	// 初始化
	int best = -MATE_VALUE;

	// 将军检测
	if (game.lastMove().ChkChs > 0)
	{
		move_sort.getAllMoves(game);
	}
	else
	{
		int val = game.getValue(alpha, beta);
		if (val >= beta)
		{
			return val;
		}
		best = val;
		if (val > alpha)
			alpha = val;
		// 生成吃子着法
		move_sort.getCapMoves(game);
	}

	// 向下搜索
	int16_t mv;
	while (mv = move_sort.nextQuiesc())
	{
		if (game.takeOneMove(mv))
		{
			int val = -quieseSearch(game, -beta, -alpha);
			game.deleteOneMove();
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
		return game.depth - MATE_VALUE;
	}
	return best;
}

// 主要变例搜索
int pvSearch(int depth, int alpha, int beta)
{
	bool flagPV = false;//主要变例搜索
	if (depth == 0)
	{
		//对于叶子节点，进行静态搜索
		return quieseSearch(Search.pos, alpha, beta);
	}

	// 无害剪裁

	// 置换表
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

	MoveSort mov;
	mov.getAllMoves(Search.pos);
	int val, mv, new_depth;
	while (mv = mov.next())
	{
		if(!Search.pos.takeOneMove(mv))
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
				break;
			}
			if (val > alpha)
			{
				alpha = val;
			}
		}
	}
	if (best == -MATE_VALUE)
		return Search.pos.depth - MATE_VALUE;
	else
	{
		// 更新置换表
		return best;
	}
}

// 根节点搜索
void searchRoot(int depth)
{
	MoveSort move_sort;
	move_sort.getAllMoves(Search.pos);
	int16_t mv;
	int new_depth, val;
	int best = -MATE_VALUE;

	while (mv = move_sort.next())
	{
		if (!Search.pos.takeOneMove(mv))
			continue;
		// 尝试性延伸
		new_depth = Search.pos.detectCheck()? depth: depth - 1;
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
	}
}

void searchMain(int depth)
{
	Search.pos.depth = 0;
	Search.pos.printForDebug();
	Search.result = -1;
	// 迭代加深
	searchRoot(depth);
	return;
}