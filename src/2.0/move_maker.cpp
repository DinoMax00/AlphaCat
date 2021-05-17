// �߷�����
#include <iostream>

#include "game.h"
#include "pregen.h"


// ���������ŷ�
int Game::genAllMoves(Move moves[])
{
	int cnt = this->genCapMoves(moves);
	cnt += this->genNonCapMoves(moves, cnt - 1);
	return cnt;
}

// �����ŷ��������ŷ�����	δ�����������
int Game::genCapMoves(Move moves[])
{
	SlideMoveStruct* p_bit;					// ָ��λ�� λ�������ָ��
	int tag = sideTag(this->cur_player);	// ��Ϸ��ɫtag
	int opptag = 48 - tag;					// �Է���ɫtag
	int src = 0, dst = 0;					// ������ֹλ��
	uint8_t* p, * p_leg;					// ָ��Ԥ���߷���ָ��
	int cnt = 0;							// �߷�������

	// �����ŷ�
	src = this->pieces[JIANG_FROM + tag];
	if (src)
	{
		p = preGen.preJiangMoves[src];
		while (*p)
		{
			dst = *p;
			// �Ƿ����
			if (this->board[dst] & opptag)	//����16/32λ�����ж�
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
				if (this->board[dst] & opptag)
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
				if ((this->board[dst] & opptag) && this->board[*p_leg] == 0)
					moves[cnt++].step = getMoveType(src, dst);
				p++;
				p_leg++;
			}
		}
	}

	// �����ŷ�
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
				if ((this->board[dst] & opptag) && this->board[*p_leg] == 0)
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
				if (this->board[dst] & opptag)
					moves[cnt++].step = getMoveType(src, dst);
				p++;
			}
		}
	}

	// �ڵ��ŷ�
	for (int i = PAO_FROM; i <= PAO_TO; i++)
	{
		src = this->pieces[tag + i];
		if (src)
		{
			// ��ȡ����
			int x = getIdxRow(src);
			int y = getIdxCol(src);

			p_bit = preGen.rowMoveTab[y - BOARD_LEFT] + this->bitRow[x];
			// ����
			dst = p_bit->PaoCap[0] + (x << 4); // x << 4 ��ȡ��ǰ����Ԫ���±�
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
			// ����
			dst = p_bit->PaoCap[1] + (x << 4);
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}

			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// ����
			dst = p_bit->PaoCap[0] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
			// ����
			dst = p_bit->PaoCap[1] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
		}
	}

	// �����ŷ�
	for (int i = JU_FROM; i <= JU_TO; i++)
	{
		src = this->pieces[tag + i];
		if (src)
		{
			// ��ȡ����
			int x = getIdxRow(src);
			int y = getIdxCol(src);

			p_bit = preGen.rowMoveTab[y - BOARD_LEFT] + this->bitRow[x];
			// ����
			dst = p_bit->JuCap[0] + (x << 4); // x << 4 ��ȡ��ǰ����Ԫ���±�
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
			// ����
			dst = p_bit->JuCap[1] + (x << 4);
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// ����
			dst = p_bit->JuCap[0] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
			// ����
			dst = p_bit->JuCap[1] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
		}
	}
	return cnt;
}

// �������ŷ�, �����ŷ�����
int Game::genNonCapMoves(Move moves[], int start)
{
	SlideMoveStruct* p_bit;					// ָ��λ�� λ�������ָ��
	int tag = sideTag(this->cur_player);	// ��Ϸ��ɫtag
	int src = 0, dst = 0;					// ������ֹλ��
	uint8_t* p, * p_leg;					// ָ��Ԥ���߷���ָ��
	int cnt = 0;							// �߷�������

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
				moves[start + cnt++].step = getMoveType(src, dst);
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
					moves[start + cnt++].step = getMoveType(src, dst);
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
					moves[start + cnt++].step = getMoveType(src, dst);
				p++;
				p_leg++;
			}
		}
	}

	// �����ŷ�
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
					moves[start + cnt++].step = getMoveType(src, dst);
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
					moves[start + cnt++].step = getMoveType(src, dst);
				p++;
			}
		}
	}

	// �ںͳ����ŷ� ��ֱ�ߣ���������
	for (int i = JU_FROM; i <= PAO_TO; i++)
	{
		src = this->pieces[tag + i];
		if (src)
		{
			// ��ȡ����
			int x = getIdxRow(src);
			int y = getIdxCol(src);

			p_bit = preGen.rowMoveTab[y - BOARD_LEFT] + this->bitRow[x];
			// ����
			dst = p_bit->NonCap[0] + (x << 4); // x << 4 ��ȡ��ǰ����Ԫ���±�
			while (dst != src)
			{
				moves[start + cnt++].step = getMoveType(src, dst);
				dst--;
			}
			// ����
			dst = p_bit->NonCap[1] + (x << 4);
			while (dst != src)
			{
				moves[start + cnt++].step = getMoveType(src, dst);
				dst++;
			}
			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// ����
			dst = p_bit->NonCap[0] + y;
			while (dst != src)
			{
				moves[start + cnt++].step = getMoveType(src, dst);
				dst -= 16;
			}
			// ����
			dst = p_bit->NonCap[1] + y;
			while (dst != src)
			{
				moves[start + cnt++].step = getMoveType(src, dst);
				dst += 16;
			}
		}
	}
	return cnt;
}


bool Game::detectCheck()
//����ֵ��Ϊ�����жϣ�����������Ķ�
{
	SlideMaskStruct* p_bitrow, * p_bitcol;
	int tag = sideTag(this->cur_player);
	int opptag = 48 - tag;
	int src = 0, dst = 0;


	src = this->pieces[JIANG_FROM + tag];
	if (!src)
		return 0;
	int x = getIdxRow(src);
	int y = getIdxCol(src);
	p_bitrow = preGen.rowMaskTab[y - BOARD_LEFT] + this->bitRow[x];
	p_bitcol = preGen.colMaskTab[x - BOARD_TOP] + this->bitCol[y];

	// ��˧����
	dst = this->pieces[JIANG_FROM + opptag];
	if (dst)
		if (y == getIdxCol(dst) && (p_bitcol->JuCap & preGen.bitColMask[dst]))
			return true;

	// ��������
	for (int i = MA_FROM; i <= MA_TO; i++)
	{
		dst = this->pieces[i + opptag];
		if (dst)
		{
			int p_leg = dst + preMaLegTab[src - dst + 256];
			if (p_leg != dst && this->board[p_leg] == 0)	//ͬʱ�ж��ܷ�Ե���+����
				return true;
		}
	}

	// ����������������������
	for (dst = src - 1; dst <= src + 1; dst += 2)
	{
		int piece = this->board[dst];
		if (pieceType[piece] == 6 && (piece & opptag))
			return true;
	}
	//����ǰ��
	{
		int piece = this->board[src - 16 + tag << 5];	//�����
		if (pieceType[piece] == 6 && (piece & opptag))
			return true;
	}

	// ���ڽ���
	for (int i = PAO_FROM; i <= PAO_TO; i++)
	{
		dst = this->pieces[i + opptag];
		if (dst)
		{
			if (x == getIdxRow(dst) && (p_bitrow->PaoCap & preGen.bitRowMask[dst]))
				return true;
			else if (y == getIdxCol(dst) && (p_bitcol->PaoCap & preGen.bitColMask[dst]))
				return true;
		}
	}

	// ��������
	for (int i = JU_FROM; i <= JU_TO; i++)
	{
		dst = this->pieces[i + opptag];
		if (dst)
		{
			if (x == getIdxRow(dst) && (p_bitrow->JuCap & preGen.bitRowMask[dst]))
				return true;
			else if (y == getIdxCol(dst) && (p_bitcol->JuCap & preGen.bitColMask[dst]))
				return true;
		}
	}
}