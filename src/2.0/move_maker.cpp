// �߷�����
#include <iostream>

#include "game.h"
#include "pregen.h"

// �������ŷ�, �����ŷ�����
int Game::genNotCapMoves(Move moves[]) 
{
	int tag = sideTag(this->cur_player);
	int src = 0, dst = 0;
	uint8_t* p, * p_leg;
	int cnt = 0;

	// �����ŷ�
	src = this->pieces[JIANG_FROM + tag];
	if (src)
	{
		p = preGen.preJiangMoves[src];
		while (*p)
		{
			dst = *p;
			// �Ƿ����
			if (this->board[dst] == 0)
				moves[cnt++].step = getMoveType(src, dst);
			p++;
		}
	}

	// ʿ���ŷ�
	for (int i = SHI_FROM; i <= SHI_TO; i++)
	{
		src = this->pieces[tag + i];
		if (src)
		{
			p = preGen.preShiMoves[src];
			while (*p)
			{
				dst = *p;
				if (this->board[dst] == 0)
					moves[cnt++].step = getMoveType(src, dst);
				p++;
			}
		}
	}

	// ����ŷ�
	for (int i = XIANG_FROM; i <= XIANG_TO; i++)
	{
		src = this->pieces[tag + i];
		if (src)
		{
			p = preGen.preXiangMoves[src];
			p_leg = preGen.preXiangLegs[src];
			while (*p)
			{
				dst = *p;
				if (this->board[dst] == 0 && this->board[*p_leg] == 0)
					moves[cnt++].step = getMoveType(src, dst);
				p++;
				p_leg++;
			}
		}
	}

	// ����ŷ�
	for (int i = MA_FROM; i <= MA_TO; i++)
	{
		src = this->pieces[tag + i];
		if (src)
		{
			p = preGen.preMaMoves[src];
			p_leg = preGen.preMaLegs[src];
			while (*p)
			{
				dst = *p;
				if (this->board[dst] == 0 && this->board[*p_leg] == 0)
					moves[cnt++].step = getMoveType(src, dst);
				p++;
				p_leg++;
			}
		}
	}

	// �����ŷ�
	for (int i = BING_FROM; i <= BING_TO; i++)
	{
		src = this->pieces[tag + i];
		if (src)
		{
			p = preGen.preBingMoves[this->cur_player][src];
			while (*p)
			{
				dst = *p;
				if (this->board[dst] == 0)
					moves[cnt++].step = getMoveType(src, dst);
				p++;
			}
		}
	}

	
	return cnt;
}
