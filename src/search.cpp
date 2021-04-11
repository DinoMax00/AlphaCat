#include "search.h"
#include "log.h"
#include<time.h>

Move searcher::getBestMove(Board &now)
{
	Move x("a0i9");
	if (now.move_vec.empty())
		return x;
	int dep = 5;
	start = clock(), end = clock();
	while (end - start < TIME_LIMIT)
	{
		int val = searchAlphaBeta(now, dep, -inf, inf, dep);
		now.generateMoves();
		x = now.move_vec[best];
		++dep;
		end = clock();
	}
	return x;
}

int searcher::searchAlphaBeta(Board& now, int depth, int alpha, int beta, int top)
{
	if (depth == 0 || clock() - start > TIME_LIMIT) {
		return now.getGameVal();
	}
	now.generateMoves();
	int size = now.move_vec.size();
	for (int i = 0; i < size; i++)
		moveInDep[depth][i] = now.move_vec[i];
	for (int i = 0; i < size; i++)
	{
		now.player ^= 1;
		now.genOneMove(moveInDep[depth][i]);
		int val = -searchAlphaBeta(now, depth - 1, -beta, -alpha, top);
		now.player ^= 1;
		now.deleteOneMove(moveInDep[depth][i]);
		
		if (val >= beta)
			return beta;
		if (val > alpha)
		{
			alpha = val;
			if (depth == top)
				best = i;
		}
	}
	return alpha;
}