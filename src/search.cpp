#include"search.h"

Move searcher::getBestMove(Board &now)
{
	Move x("a0i9");
	if (now.move_vec.empty())
		return x;
	int val = searchAlphaBeta(now, MAX_SEARCH_DEPTH, -inf, inf);
	return now.move_vec[best];
}

int searcher::searchAlphaBeta(Board& now, int depth, int alpha, int beta)
{
	if (depth == 0)
		return now.gameVal;
	now.generateMoves();
	int size = now.move_vec.size();
	for (int i = 0; i < size; i++)
	{
		Move temp = now.move_vec[i];
		now.genOneMove(temp);
		int val = -searchAlphaBeta(now, depth - 1, -beta, -alpha);
		now.deleteOneMove(temp);
		now.generateMoves();
		if (val >= beta)
			return beta;
		if (val > alpha)
		{
			alpha = val;
			if (depth == MAX_SEARCH_DEPTH)
				best = i;
		}
	}
	return alpha;
}