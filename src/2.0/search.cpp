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

// ��Ҫ��������
int pvSearch(int depth, int alpha, int beta)
{
	bool flagPV = false;//��Ҫ��������
	if (depth == 0)
	{
		//����Ҷ�ӽڵ㣬���о�̬����
		return quieseSearch(Search.pos, alpha, beta);
	}

	// �޺�����

	// �û���
	// ������� ���ع�ֵ
	if (Search.pos.depth >= LIMIT_DEPTH)
		return Search.pos.getValue(alpha, beta);
	
	int best = -MATE_VALUE;
	//// �ڲ�������������
	//if (depth > 2)
	//{
	//	int val = pvSearch(Search.pos, depth / 2, alpha, beta, max_deep);
	//	if (val <= alpha)
	//		val = pvSearch(Search.pos, depth / 2, -MATE_VALUE, beta, max_deep);
	//}

	MoveSort mov;
	mov.getAllMoves(Search.pos);
	int val, mv, new_depth;
	while (mv = mov.next())
	{
		if(!Search.pos.takeOneMove(mv))
			continue;
		// ����ѡ������
		new_depth = Search.pos.detectCheck() ? depth : depth - 1;
		if (best == -MATE_VALUE)
			val = -pvSearch(new_depth, -beta, -alpha);
		else 
		{
			val = -pvSearch(new_depth, -alpha - 1, -alpha);
			if (val > alpha && val < beta) 
				val = -pvSearch(new_depth, -beta, -alpha);
		}

		Search.pos.deleteOneMove();

		// alpha_beta�߽��ж�
		if (val > best)
		{
			best = val;
			if (val >= beta)
			{
				break;
			}
			if (val > alpha)
			{
				alpha = val;
			}
		}
	}
	if (best == -MATE_VALUE)
		return Search.pos.depth - MATE_VALUE;
	else
	{
		// �����û���
		return best;
	}
}

// ���ڵ�����
void searchRoot(int depth)
{
	MoveSort move_sort;
	move_sort.getAllMoves(Search.pos);
	int16_t mv;
	int new_depth, val;
	int best = -MATE_VALUE;

	while (mv = move_sort.next())
	{
		if (!Search.pos.takeOneMove(mv))
			continue;
		// ����������
		new_depth = Search.pos.detectCheck()? depth: depth - 1;
		// ��Ҫ��������
		if (best == -MATE_VALUE)
			val = -pvSearch(new_depth, -MATE_VALUE, MATE_VALUE);
		else
		{
			val = -pvSearch(new_depth, -best - 1, -best);
			if (val > best)
				val = -pvSearch(new_depth, -MATE_VALUE, -best);
		}
		Search.pos.deleteOneMove();

		// ���������ŷ�
		if (val > best)
		{
			best = val;
			Search.result = mv;
		}
	}
}

void searchMain(int depth)
{
	Search.pos.depth = 0;
	Search.pos.printForDebug();
	Search.result = -1;
	// ��������
	searchRoot(depth);
	return;
}