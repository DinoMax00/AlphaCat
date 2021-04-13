#include "search.h"
#include "log.h"
#include<time.h>

Move searcher::getBestMove(Board& now)
{
	Move x("a0i9");
	if (now.move_vec.empty())
		return x;
	start = end = clock();
	// while ()
	// 非迭代加深版
	max_deep = 1;
	while (end - start < TIME_LIMIT)
	{
		int val = searchAlphaBeta(now, max_deep, -inf, inf);
		now.generateMoves();
		if (end - start < TIME_LIMIT)
			x = now.move_vec[best];
		else
			break;
		++max_deep;
		end = clock();
	}
	Log debug;
	debug.info(int(max_deep));
	return x;
}

int searcher::searchAlphaBeta(Board& now, int depth, int alpha, int beta)
{
	int hashf = 1, val = now.hashNum.searchFromTransTable(now.Zobrist, depth, now.player, alpha, beta);
	if (val != -10000000)
		return val;

	if (depth == 0 || (now.board[now.pos_of_kings[now.player]] != R_JIANG && now.board[now.pos_of_kings[now.player]] != B_JIANG)) 
	{
		val = now.getGameVal();
		now.hashNum.saveInTransTable(now.Zobrist, depth, now.player, 0, val);
		return val;
	}

	now.generateMoves();
	int size = now.move_vec.size();
	if (!size)
		return -1000000;
	for (int i = 0; i < size; i++)
		moveInDep[depth][i] = now.move_vec[i];
	for (int i = 0; i < size; i++)
	{
		end = clock();
		if (end - start >= TIME_LIMIT)
			break;
		now.player ^= 1;
		now.genOneMove(moveInDep[depth][i]);
		val = -searchAlphaBeta(now, depth - 1, -beta, -alpha);
		now.player ^= 1;
		now.deleteOneMove(moveInDep[depth][i]);
		if (val >= beta)
		{
			now.hashNum.saveInTransTable(now.Zobrist, depth, now.player, 2, beta);
			return beta;
		}
			
		if (val > alpha)
		{
			hashf = 0;
			alpha = val;
			if (depth == max_deep)
				best = i;
		}
	}
	now.hashNum.saveInTransTable(now.Zobrist, depth, now.player, hashf, alpha);
	return alpha;
}