#include <iostream>

#include "ucci.h"
#include "move.h"
#include "search.h"

SearchStruct Search;

// ��̬����
int quieseSearch(Game& game, int alpha, int beta)
{
	MoveSort move_sort;

	// �޺�����

	// ������ȷ��ؾ����ֵ
	if (game.depth >= LIMIT_DEPTH)
		return game.getValue(alpha, beta);

	// ��ʼ��
	int best = -MATE_VALUE;

	// �������
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
		// ���ɳ����ŷ�
		move_sort.getCapMoves(game);
	}

	// ��������
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

	// ���ط�ֵ
	if (best == -MATE_VALUE)
	{
		return game.depth - MATE_VALUE;
	}
	return best;
}


int searchAlphabeta(Game& now, int depth, int alpha, int beta, int max_deep)
{
	bool flagPV = false;//��Ҫ��������
	if (depth == 0)
	{
		//����Ҷ�ӽڵ㣬���о�̬����
		return quieseSearch(now, alpha, beta);
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

		if (flagPV)
		{
			val = -searchAlphabeta(now, depth - 1, -alpha - 1, -alpha, max_deep);
			if (val > alpha && val < beta)
				val = -searchAlphabeta(now, depth - 1, -beta, -alpha, max_deep);
		}
		else
			val = -searchAlphabeta(now, depth - 1, -beta, -alpha, max_deep);
		now.deleteOneMove();

		if (val >= beta)
			return beta;
		if (val > alpha)
		{
			alpha = val;
			flagPV = true;
			if (depth == max_deep) {
				Search.result = mv;
			}
		}
	}

	if (isEmpty) return -1e7;
	return alpha;
}

void SearchMain(int nDepth)
{
	int dep = nDepth;
	
	searchAlphabeta(Search.pos, dep, -MATE_VALUE, MATE_VALUE, dep);
	return;
}