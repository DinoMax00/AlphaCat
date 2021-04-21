#include "search.h"
#include "log.h"
#include "opbook.h"
#include<time.h>

Move searcher::getBestMove(Board& now)
{
	opBook opb;
	Move x("a0i9");
	if (now.move_vec.empty())
		return x;
	start = end = clock();
	// while ()
	// �ǵ��������
	int opb_search = opb.opBook_search(now.Zobrist);
	if (opb_search != -1)
		return Move(opb_search / 256 + 1, opb_search % 256 + 1);
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
	debug.info(int(max_deep - 1));
	return x;
}

int searcher::searchAlphaBeta(Board& now, int depth, int alpha, int beta)
{
	int hashf = hashAlpha, val = now.hashNum.searchFromTransTable(now.Zobrist, depth, now.player, alpha, beta);
	if (val != -10000000)
		return val;

	if (depth == 0 || (now.board[now.pos_of_kings[now.player]] != R_JIANG && now.board[now.pos_of_kings[now.player]] != B_JIANG))
	{
		val = now.getGameVal();
		now.hashNum.saveInTransTable(now.Zobrist, depth, now.player, hashExact, val);
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
			now.hashNum.saveInTransTable(now.Zobrist, depth, now.player, hashBeta, beta);
			return beta;
		}

		if (val > alpha)
		{
			hashf = hashExact;
			alpha = val;
			if (depth == max_deep)
				best = i;
		}
	}
	now.hashNum.saveInTransTable(now.Zobrist, depth, now.player, hashf, alpha);
	return alpha;
}