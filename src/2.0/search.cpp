#include <iostream>
#include <Windows.h>

#include "ucci.h"
#include "move.h"
#include "hash.h"
#include "search.h"
#include "log.h"
SearchStruct Search;

// �޺�����
inline int harmlessPruning(int beta)
{
	int val;

	// ɱ�岽������
	val = Search.pos.depth - MATE_VALUE;
	if (val >= beta)
		return val;

	// ѭ���������

	// �ظ�����
	int val_rep = Search.pos.detectCircle();
	if (val_rep > 0)
		return Search.pos.circleVal(val_rep);

	return -MATE_VALUE;
}

// ��̬����
int quieseSearch(int alpha, int beta)
{
	MoveSort move_sort;
	int  val;

	// �޺�����
	val = harmlessPruning(beta);
	if (val > -MATE_VALUE)
	{
		return val;
	}
	

	// ������ȷ��ؾ����ֵ
	if (Search.pos.depth >= LIMIT_DEPTH)
		return Search.pos.getValue(alpha, beta);

	// ��ʼ��
	int best = -MATE_VALUE;

	// �������
	if (Search.pos.lastMove().ChkChs > 0)
	{
		move_sort.getAllMoves(Search.pos);
	}
	else
	{
		val = Search.pos.getValue(alpha, beta);
		if (val >= beta)
			return val;
		best = val;
		if (val > alpha)
			alpha = val;
		// ���ɳ����ŷ�
		move_sort.getCapMoves(Search.pos);
	}

	// ��������
	int16_t mv;
	while (mv = move_sort.nextQuiesc())
	{
		if (Search.pos.takeOneMove(mv))
		{
			int val = -quieseSearch(-beta, -alpha);
			Search.pos.deleteOneMove();
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
		return Search.pos.depth - MATE_VALUE;
	}
	return best;
}

// �㴰����ȫ��������
int cutSearch(int depth, int beta, bool no_null = false)
{
	int val, mvhash;
	MoveSort move_sort;

	// Ҷ�ӽڵ㴦���þ�̬����
	if (depth <= 0)
	{
		return quieseSearch(beta - 1, beta);
	}

	// �޺�����
	val = harmlessPruning(beta);
	if (val > -MATE_VALUE)
		return val;

	// �û�����
	val = getHashTable(Search.pos.zobrist, beta - 1, beta, depth, mvhash);
	if (val > -MATE_VALUE)
	{
		return val;
	}

	// ������� ֱ�ӷ�������ֵ
	if (Search.pos.depth >= LIMIT_DEPTH)
	{
		return Search.pos.getValue(beta - 1, beta);
	}

	// ���ż���
	if (!no_null && Search.pos.lastMove().ChkChs <= 0 && Search.pos.nullOk())
	{

	}
} 

// ��Ҫ��������
int pvSearch(int depth, int alpha, int beta)
{
	int val;
	bool flagPV = false;//��Ҫ��������
	if (depth == 0)
	{
		//����Ҷ�ӽڵ㣬���о�̬����
		return quieseSearch(alpha, beta);
	}

	// �޺�����
	val = harmlessPruning(beta);
	if (val > -MATE_VALUE)
		return val;

	// �û�����
	int hashflag = FLAG_ALPHA;
	int vlhash, mvhash;
	vlhash = getHashTable(Search.pos.zobrist, alpha, beta, depth, mvhash);
	if (vlhash > -MATE_VALUE)
		return vlhash;

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

	MoveSort move_sort;
	move_sort.getAllMoves(Search.pos);
	int mv, new_depth;
	while (mv = move_sort.next())
	{
		if (!Search.pos.takeOneMove(mv))
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
				hashflag = FLAG_BETA;
				break;
			}
			if (val > alpha)
			{
				hashflag = FLAG_PV;
				alpha = val;
			}
		}
	}

	if (best == -MATE_VALUE)
		return Search.pos.depth - MATE_VALUE;
	else
	{
		// �����û���
		recordHashTable(Search.pos.zobrist, hashflag, best, depth, mv);
		//mv��㴫��һ����������
		return best;
	}
}

// ���ڵ�����
int searchRoot(int depth)
{
	MoveSort move_sort;
	move_sort.getAllMoves(Search.pos);
	int16_t mv;
	int new_depth, val;
	int best = -MATE_VALUE;

	// �������ڵ����ŷ�
	while (mv = move_sort.next())
	{
		if (!Search.pos.takeOneMove(mv))
			continue;
		
		// ����������
		new_depth = Search.pos.detectCheck() ? depth : depth - 1;
		
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

		// ��ʱ
		if (Search.stop)
		{
			return best;
		}

		// ���������ŷ�
		if (val > best)
		{
			best = val;
			Search.result = mv;
		}

		if (GetTickCount64() - Search.cur_time > Search.time_limit)
		{
			Search.stop = true;
		}
	}
	return best;
}

void initSearch()
{
	Search.pos.depth = 0;
	Search.result = -1;
	Search.time_limit = 1500;
	Search.stop = false;
}

void searchMain(int depth)
{
	int val, val_last;
	int16_t best_move = -1;
	uint64_t time_temp;

	// ��ʼ��
	initSearch();
	// ���ֿ�

	// ��ʼ��������

	// ��չ�ϣ�� ��ʷ�� ɱ���ŷ���
	clearHashTable();
	clearHistory();

	// ��ʼ��ʱ
	Search.cur_time = GetTickCount64();

	// ��������
	for (int i = 1; i <= depth; i++)
	{
		val = searchRoot(i);

		if (Search.stop)
		{
			std::cout << "��������: " << i << std::endl;
			Log().info(i);
			break;
		}

		best_move = Search.result;

		// �ѵ�ɱ�� ��ֹ����
		if (val > WIN_VALUE || val < -WIN_VALUE)
		{
			break;
		}
	}	
	Search.result = best_move;
	return;
}