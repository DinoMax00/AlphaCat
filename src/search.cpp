#include"search.h"

Move searcher::getBestMove(Board &now)
{
	now.printBoardForDebug();
	Move x("a0i9");
	if (now.move_vec.empty())
		return x;
	int val = searchAlphaBeta(now, MAX_SEARCH_DEPTH, -inf, inf);
	now.generateMoves();
	x = now.move_vec[best];
	return x;
}

int searcher::searchAlphaBeta(Board& now, int depth, int alpha, int beta)
{
	if (depth == 0)
		return now.gameVal;
	now.generateMoves();
	int size = now.move_vec.size();
	for (int i = 0; i < size; i++)
		moveInDep[depth][i] = now.move_vec[i];
	for (int i = 0; i < size; i++)
	{
		now.genOneMove(moveInDep[depth][i]);
		int val = -searchAlphaBeta(now, depth - 1, -beta, -alpha);
		now.deleteOneMove(moveInDep[depth][i]);
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