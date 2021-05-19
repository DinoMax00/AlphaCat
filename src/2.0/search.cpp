
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
	int val;
	while (1)
	{
		int mv = mov.next();
		if (!mv) break;

		bool flag = now.takeOneMove(mv);
		if (!flag) continue;

		isEmpty = 0;
		val = -searchAlphabeta(now, depth - 1, -beta, -alpha, max_deep);
		now.deleteOneMove();

		if (val >= beta)
			return beta;
		if (val > alpha)
		{
			alpha = val;
			if (depth == max_deep)
				Search.result = mv;
		}
	}

	if (isEmpty) return -1e7;
	return alpha;
}

void SearchMain(int nDepth)
{
	int dep = nDepth;
	const int inf = 1e9;
	Search.result = searchAlphabeta(Search.pos, dep, -inf, inf, dep);
	return;
}