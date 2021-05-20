// �߷�����
#include <iostream>
#include "game.h"
#include "pregen.h"


bool Game::isProtected(int tag, int src, int except)
{
	int dst = 0;
	int x = getIdxRow(src);
	int y = getIdxCol(src);
	SlideMaskStruct* p_bitrow, * p_bitcol;
	p_bitrow = preGen.rowMaskTab[y - BOARD_LEFT] + this->bitRow[x];
	p_bitcol = preGen.colMaskTab[x - BOARD_TOP] + this->bitCol[y];

	if ((src & 0x80) != (tag - 16) << 4)	//����λ�ڱ����
	{
		//˧�ı���
		dst = this->pieces[tag + JIANG_FROM];
		if (dst && dst != except && LegalMoveTab[dst - src + 256] == 1)
			return true;
		//ʿ�ı���
		for (int i = SHI_FROM; i <= SHI_TO; i++)
		{
			dst = this->pieces[tag + i];
			if (dst && dst != except && LegalMoveTab[dst - src + 256] == 2)
				return true;
		}
		//��ı���
		for (int i = XIANG_FROM; i <= XIANG_TO; i++)
		{
			dst = this->pieces[tag + i];
			if (dst && dst != except && LegalMoveTab[dst - src + 256] == 3 && !this->board[(src + dst) / 2])
				return true;
		}
	}
	else	//����λ�ڶԷ����
	{
		//���ı���--����
		for (dst = src - 1; dst <= src + 1; dst += 2)
		{
			int piece = this->board[dst];
			if (dst != except && pieceType[piece] == 6 && (piece & tag))
				return true;
		}
	}
	//��ı���
	for (int i = MA_FROM; i <= MA_TO; i++)
	{
		dst = this->pieces[tag + i];
		if (dst && dst != except)
		{
			int p_leg = dst + preMaLegTab[src - dst + 256];
			if (p_leg != dst && !this->board[p_leg])
				return true;
		}
	}
	//���ı���
	for (int i = JU_FROM; i <= JU_TO; i++)
	{
		dst = this->pieces[tag + i];
		if (dst && dst != except)
		{
			if (x == getIdxRow(dst) && (p_bitrow->ju_cap & preGen.bitRowMask[dst]))
				return true;
			else if (y == getIdxCol(dst) && (p_bitcol->ju_cap & preGen.bitColMask[dst]))
				return true;
		}
	}
	//�ڵı���
	for (int i = PAO_FROM; i <= PAO_TO; i++)
	{
		dst = this->pieces[tag + i];
		if (dst && dst != except)
		{
			if (x == getIdxRow(dst) && (p_bitrow->pao_cap & preGen.bitRowMask[dst]))
				return true;
			else if (y == getIdxCol(dst) && (p_bitcol->pao_cap & preGen.bitColMask[dst]))
				return true;
		}
	}
	//���ı���--ǰ��
	{
		dst = src - 16 + ((tag / 16 - 1) << 5);
		int piece = this->board[dst];
		if (dst != except && pieceType[piece] == 6 && (piece & tag))
			return true;
	}
	return false;
}
int Game::moveJudge(int opptag, int src, int dst)
{
	int mvv = pieceValue[this->board[dst]];
	int lva = (isProtected(opptag, dst) ? pieceValue[this->board[src]] : 0);
	if (mvv >= lva)
		return mvv - lva + 1;
	else if (mvv >= 3 || ((dst & 0x80) == (opptag - 16) << 4))
		return 1;
	return 0;
}

// ���������ŷ�
int Game::genAllMoves(Move moves[])
{
	int cnt = this->genCapMoves(moves);
	cnt += this->genNonCapMoves(moves + cnt);
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
	src = this->pieces[tag + JIANG_FROM];
	if (src)
	{
		p = preGen.preJiangMoves[src];
		while (*p)
		{
			dst = *p;
			// �Ƿ����
			if (this->board[dst] & opptag)	//����16/32λ�����ж�
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
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
				{
					moves[cnt].step = getMoveType(src, dst);
					moves[cnt++].value = moveJudge(opptag, src, dst);
				}
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
				{
					moves[cnt].step = getMoveType(src, dst);
					moves[cnt++].value = moveJudge(opptag, src, dst);
				}
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
				if ((this->board[dst] & opptag) && this->board[*p_leg] == 0)
				{
					moves[cnt].step = getMoveType(src, dst);
					moves[cnt++].value = moveJudge(opptag, src, dst);
				}
				p++;
				p_leg++;
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
			dst = p_bit->juCap[0] + (x << 4); // x << 4 ��ȡ��ǰ����Ԫ���±�
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
			// ����
			dst = p_bit->juCap[1] + (x << 4);
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// ����
			dst = p_bit->juCap[0] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
			// ����
			dst = p_bit->juCap[1] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
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
			dst = p_bit->paoCap[0] + (x << 4); // x << 4 ��ȡ��ǰ����Ԫ���±�
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
			// ����
			dst = p_bit->paoCap[1] + (x << 4);
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}

			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// ����
			dst = p_bit->paoCap[0] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
			// ����
			dst = p_bit->paoCap[1] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
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
	return cnt;
}

// �������ŷ�, �����ŷ�����
int Game::genNonCapMoves(Move moves[])
{
	SlideMoveStruct* p_bit;					// ָ��λ�� λ�������ָ��
	int tag = sideTag(this->cur_player);	// ��Ϸ��ɫtag
	int src = 0, dst = 0;					// ������ֹλ��
	uint8_t* p, * p_leg;					// ָ��Ԥ���߷���ָ��
	int cnt = 0;							// �߷�������

	// �����ŷ�
	src = this->pieces[tag + JIANG_FROM];
	if (src)
	{
		p = preGen.preJiangMoves[src];
		while (*p)
		{
			dst = *p;
			// �Ƿ����
			if (this->board[dst] == 0)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = 0;
			}
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
				{
					moves[cnt].step = getMoveType(src, dst);
					moves[cnt++].value = 0;
				}
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
				{
					moves[cnt].step = getMoveType(src, dst);
					moves[cnt++].value = 0;
				}
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
				{
					moves[cnt].step = getMoveType(src, dst);
					moves[cnt++].value = 0;
				}
				p++;
				p_leg++;
			}
		}
	}
	// �����ڵ��ŷ� ��ֱ�ߣ���������
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
			dst = p_bit->nonCap[0] + (x << 4); // x << 4 ��ȡ��ǰ����Ԫ���±�
			while (dst != src)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = 0;
				dst--;
			}
			// ����
			dst = p_bit->nonCap[1] + (x << 4);
			while (dst != src)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = 0;
				dst++;
			}
			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// ����
			dst = p_bit->nonCap[0] + y;
			while (dst != src)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = 0;
				dst -= 16;
			}
			// ����
			dst = p_bit->nonCap[1] + y;
			while (dst != src)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = 0;
				dst += 16;
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
				{
					moves[cnt].step = getMoveType(src, dst);
					moves[cnt++].value = 0;
				}
				p++;
			}
		}
	}
	return cnt;
}


int Game::detectCheck(bool simple)
//����ֵΪ0/1/����������ֵ������ͬʱ����ʱ����1����simpleģʽ�·���ֵ��Ϊ0/1
{
	SlideMaskStruct* p_bitrow, * p_bitcol;
	int tag = sideTag(this->cur_player);
	int opptag = 48 - tag;
	int src = 0, dst = 0;
	int record = 0;

	src = this->pieces[tag + JIANG_FROM];
	if (!src)
		return 0;
	int x = getIdxRow(src);
	int y = getIdxCol(src);
	p_bitrow = preGen.rowMaskTab[y - BOARD_LEFT] + this->bitRow[x];
	p_bitcol = preGen.colMaskTab[x - BOARD_TOP] + this->bitCol[y];

	// ��˧����
	dst = this->pieces[opptag + JIANG_FROM];
	if (dst)
		if (y == getIdxCol(dst) && (p_bitcol->ju_cap & preGen.bitColMask[dst]))
		{
			if (simple || record)
				return true;
			else
				record = opptag + JIANG_FROM;
		}

	// ������
	for (int i = MA_FROM; i <= MA_TO; i++)
	{
		dst = this->pieces[opptag + i];
		if (dst)
		{
			int p_leg = dst + preMaLegTab[src - dst + 256];
			if (p_leg != dst && this->board[p_leg] == 0)	//ͬʱ�ж��ܷ�Ե���+����
			{
				if (simple || record)
					return true;
				else
					record = opptag + i;
			}
		}
	}
	// ��������
	for (int i = JU_FROM; i <= JU_TO; i++)
	{
		dst = this->pieces[opptag + i];
		if (dst)
		{
			if (x == getIdxRow(dst) && (p_bitrow->ju_cap & preGen.bitRowMask[dst]))
			{
				if (simple || record)
					return true;
				else
					record = opptag + i;
			}
			else if (y == getIdxCol(dst) && (p_bitcol->ju_cap & preGen.bitColMask[dst]))
			{
				if (simple || record)
					return true;
				else
					record = opptag + i;
			}
		}
	}
	// ���ڽ���
	for (int i = PAO_FROM; i <= PAO_TO; i++)
	{
		dst = this->pieces[opptag + i];
		if (dst)
		{
			if (x == getIdxRow(dst) && (p_bitrow->pao_cap & preGen.bitRowMask[dst]))
			{
				if (simple || record)
					return true;
				else
					record = opptag + i;
			}
			else if (y == getIdxCol(dst) && (p_bitcol->pao_cap & preGen.bitColMask[dst]))
			{
				if (simple || record)
					return true;
				else
					record = opptag + i;
			}
		}
	}
	// ������������������
	for (dst = src - 1; dst <= src + 1; dst += 2)
	{
		int piece = this->board[dst];
		if (pieceType[piece] == 6 && (piece & opptag))
		{
			if (simple || record)
				return true;
			else
				for (int i = BING_FROM; i <= BING_TO; i++)
					if (pieces[opptag + i] == dst)
					{
						record = opptag + i;
						break;
					}
		}
	}
	//����������ǰ��
	{
		int piece = this->board[src - 16 + ((tag / 16 - 1) << 5)];	//�����
		if (pieceType[piece] == 6 && (piece & opptag))
		{
			if (simple || record)
				return true;
			else
				for (int i = BING_FROM; i <= BING_TO; i++)
					if (pieces[opptag + i] == dst)
					{
						record = opptag + i;
						break;
					}
		}
	}
	return record;
}