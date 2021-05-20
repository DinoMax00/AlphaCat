#include "ucci.h"
#include "move.h"
#include "search.h"

SearchStruct Search;

int searchAlphabeta(Game& now, int depth, int alpha, int beta, int max_deep)
{
	if (depth == 0)
	{
		int val = now.getEva();
		return val;
	}

	MoveSort mov;
	mov.getAllMoves(now);
	bool isEmpty = 1;
	int val, mv;
	while (mv = mov.next())
	{
		if(!now.takeOneMove(mv))
			continue;
		isEmpty = 0;
		val = -searchAlphabeta(now, depth - 1, -beta, -alpha, max_deep);
		now.deleteOneMove();

		if (val >= beta)
			return beta;
		if (val > alpha)
		{
			alpha = val;
			if (depth == max_deep) {
				Search.result = mv;
			}
		}
	}

	if (isEmpty) return -10000000;
	return alpha;
}

void SearchMain(int nDepth)
{
	int dep = nDepth;
	const int inf = 1000000000;
	searchAlphabeta(Search.pos, dep, -inf, inf, dep);
	return;
}