#include"search.h"
#include<time.h>

Move searcher::getBestMove(Board &now)
{
	now.printBoardForDebug();
	Move x("a0i9");
	if (now.move_vec.empty())
		return x;
	int dep = 1, start = clock(), end = clock();
	while (end - start < 1000)
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
	if (depth == 0)
		return now.gameVal;
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