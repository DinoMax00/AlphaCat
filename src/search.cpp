#include "search.h"
#include "log.h"
#include<time.h>

Move searcher::getBestMove(Board& now)
{
	Move x("a0i9");
	if (now.move_vec.empty())
		return x;
	// start = end = clock();
	// while (end - start < TIME_LIMIT)
	// 非迭代加深版
	max_deep = 5;
	while (max_deep<6)
	{
		int val = searchAlphaBeta(now, max_deep, -inf, inf);
		now.generateMoves();
		x = now.move_vec[best];
		++max_deep;
	}
	return x;
}

int searcher::searchAlphaBeta(Board& now, int depth, int alpha, int beta)
{
	if (depth == 0 || (now.board[now.pos_of_kings[now.player]] != R_JIANG && now.board[now.pos_of_kings[now.player]] != B_JIANG)) {
		return now.getGameVal();
	}
	now.generateMoves();
	int size = now.move_vec.size();
	if (!size)
		return -1000000;
	for (int i = 0; i < size; i++)
		moveInDep[depth][i] = now.move_vec[i];
	for (int i = 0; i < size; i++)
	{
		int val;
		now.player ^= 1;
		now.genOneMove(moveInDep[depth][i]);
		val = -searchAlphaBeta(now, depth - 1, -beta, -alpha);
		now.player ^= 1;
		now.deleteOneMove(moveInDep[depth][i]);
		if (val >= beta)
			return beta;
		if (val > alpha)
		{
			alpha = val;
			if (depth == max_deep)
				best = i;
		}
	}
	return alpha;
}