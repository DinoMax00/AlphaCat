// 走法生成
#include <iostream>
#include "game.h"
#include "pregen.h"

bool Game::legalMove(uint16_t mv)
{
	int src, dst, tag, pc_moved, pc_captured, leg, x, y;

	tag = sideTag(this->cur_player);
	src = getSrc(mv);
	dst = getDst(mv);
	pc_moved = this->board[src];
	pc_captured = this->board[dst];
	
	// 检查要走的子是否存在
	if (!(pc_moved & tag))
		return false;

	// 检查吃到的是否是对方棋子
	if (pc_captured & tag)
		return false;

	switch (pc_moved & 15)
	{
		case JIANG_FROM:
			return inHome(dst) & jiangSpan(src, dst);
		case SHI_FROM:
		case SHI_TO:
			return inHome(dst) & shiSpan(src, dst);
		case XIANG_FROM:
		case XIANG_TO:
			return sameSide(src, dst) & xiangSpan(src, dst) & !this->board[(src + dst) >> 1];
		case MA_FROM:
		case MA_TO:
			leg = src + preMaLegTab[dst - src + 256];
			return leg != src && !this->board[leg];
		case JU_FROM:
		case JU_TO:
			x = getIdxCol(src);
			y = getIdxRow(src);
			if (x == getIdxCol(dst))
			{
				if (pc_captured)
					return (preGen.colMaskTab[y - BOARD_TOP] + bitCol[x])->ju_cap & preGen.bitColMask[dst];
				else
					return (preGen.colMaskTab[y - BOARD_TOP] + bitCol[x])->non_cap & preGen.bitColMask[dst];
			}
			else if (y == getIdxRow(dst))
			{
				if (pc_captured)
					return (preGen.rowMaskTab[x - BOARD_LEFT] + bitRow[y])->ju_cap & preGen.bitRowMask[dst];
				else
					return (preGen.rowMaskTab[x - BOARD_LEFT] + bitRow[y])->non_cap & preGen.bitRowMask[dst];
			}
			else
				return false;
		case PAO_FROM:
		case PAO_TO:
			x = getIdxCol(src);
			y = getIdxRow(src);
			if (x == getIdxCol(dst))
			{
				if (pc_captured)
					return (preGen.colMaskTab[y - BOARD_TOP] + bitCol[x])->pao_cap & preGen.bitColMask[dst];
				else
					return (preGen.colMaskTab[y - BOARD_TOP] + bitCol[x])->non_cap & preGen.bitColMask[dst];
			}
			else if (y == getIdxRow(dst))
			{
				if (pc_captured)
					return (preGen.rowMaskTab[x - BOARD_LEFT] + bitRow[y])->pao_cap & preGen.bitRowMask[dst];
				else
					return (preGen.rowMaskTab[x - BOARD_LEFT] + bitRow[y])->non_cap & preGen.bitRowMask[dst];
			}
			else
				return false;
		default:
			if (awayHalf(dst, this->cur_player) & (dst == src - 1 || dst == src + 1))
			{
				return true;
			}
			else {
				return dst == (src - 16 + ((!this->cur_player) << 5));
			}
	}

}

bool Game::isProtected(int tag, int src, int except)
{
	int dst = 0;
	int x = getIdxRow(src);
	int y = getIdxCol(src);
	SlideMaskStruct* p_bitrow, * p_bitcol;
	p_bitrow = preGen.rowMaskTab[y - BOARD_LEFT] + this->bitRow[x];
	p_bitcol = preGen.colMaskTab[x - BOARD_TOP] + this->bitCol[y];

	if ((src & 0x80) != (tag - 16) << 3)	//棋子位于本半边
	{
		if (inHome(src))
		{
			//帅的保护
			dst = this->pieces[tag + JIANG_FROM];
			if (dst && dst != except && LegalMoveTab[dst - src + 256] == 1)
				return true;
			//士的保护
			for (int i = SHI_FROM; i <= SHI_TO; i++)
			{
				dst = this->pieces[tag + i];
				if (dst && dst != except && LegalMoveTab[dst - src + 256] == 2)
					return true;
			}
		}
		//相的保护
		for (int i = XIANG_FROM; i <= XIANG_TO; i++)
		{
			dst = this->pieces[tag + i];
			if (dst && dst != except && LegalMoveTab[dst - src + 256] == 3 && !this->board[(src + dst) / 2])
				return true;
		}
	}
	else	//棋子位于对方半边
	{
		//兵的保护--左右
		for (dst = src - 1; dst <= src + 1; dst += 2)
		{
			int piece = this->board[dst];
			if (dst != except && pieceType[piece] == 6 && (piece & tag))
				return true;
		}
	}
	//马的保护
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
	//车的保护
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
	//炮的保护
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
	//兵的保护-后方
	{
		dst = src + 16 - ((tag / 16 - 1) << 5);
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
	else if (mvv >= 3 || ((dst & 0x80) == (opptag - 16) << 3))
		return 1;
	return 0;
}

// 生成所有着法
int Game::genAllMoves(Move moves[])
{
	int cnt = this->genCapMoves(moves);
	cnt += this->genNonCapMoves(moves + cnt);
	return cnt;
}

// 吃子着法，返回着法数量	未来需加入评分
int Game::genCapMoves(Move moves[])
{
	SlideMoveStruct* p_bit;					// 指向位行 位列数组的指针
	int tag = sideTag(this->cur_player);	// 游戏角色tag
	int opptag = 48 - tag;					// 对方角色tag
	int src = 0, dst = 0;					// 走子起止位置
	uint8_t* p, * p_leg;					// 指向预置走法的指针
	int cnt = 0;							// 走法计数器

	// 将的着法
	src = this->pieces[tag + JIANG_FROM];
	if (src)
	{
		p = preGen.preJiangMoves[src];
		while (*p)
		{
			dst = *p;
			// 是否吃子
			if (this->board[dst] & opptag)	//借助16/32位运算判断
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
			p++;
		}
	}

	// 士的着法
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

	// 相的着法
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

	// 马的着法
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

	// 车的着法
	for (int i = JU_FROM; i <= JU_TO; i++)
	{
		src = this->pieces[tag + i];
		if (src)
		{
			// 获取行列
			int x = getIdxRow(src);
			int y = getIdxCol(src);

			p_bit = preGen.rowMoveTab[y - BOARD_LEFT] + this->bitRow[x];
			// 向右
			dst = p_bit->juCap[0] + (x << 4); // x << 4 获取当前行首元素下标
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
			// 向左
			dst = p_bit->juCap[1] + (x << 4);
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// 向下
			dst = p_bit->juCap[0] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
			// 向上
			dst = p_bit->juCap[1] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
		}
	}
	// 炮的着法
	for (int i = PAO_FROM; i <= PAO_TO; i++)
	{
		src = this->pieces[tag + i];
		if (src)
		{
			// 获取行列
			int x = getIdxRow(src);
			int y = getIdxCol(src);

			p_bit = preGen.rowMoveTab[y - BOARD_LEFT] + this->bitRow[x];
			// 向右
			dst = p_bit->paoCap[0] + (x << 4); // x << 4 获取当前行首元素下标
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
			// 向左
			dst = p_bit->paoCap[1] + (x << 4);
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}

			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// 向下
			dst = p_bit->paoCap[0] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
			// 向上
			dst = p_bit->paoCap[1] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = moveJudge(opptag, src, dst);
			}
		}
	}
	// 兵的着法
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

// 不吃子着法, 返回着法数量
int Game::genNonCapMoves(Move moves[])
{
	SlideMoveStruct* p_bit;					// 指向位行 位列数组的指针
	int tag = sideTag(this->cur_player);	// 游戏角色tag
	int src = 0, dst = 0;					// 走子起止位置
	uint8_t* p, * p_leg;					// 指向预置走法的指针
	int cnt = 0;							// 走法计数器

	// 将的着法
	src = this->pieces[tag + JIANG_FROM];
	if (src)
	{
		p = preGen.preJiangMoves[src];
		while (*p)
		{
			dst = *p;
			// 是否吃子
			if (this->board[dst] == 0)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = 0;
			}
			p++;
		}
	}

	// 士的着法
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

	// 相的着法
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

	// 马的着法
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
	// 车和炮的着法 走直线，不作区分
	for (int i = JU_FROM; i <= PAO_TO; i++)
	{
		src = this->pieces[tag + i];
		if (src)
		{
			// 获取行列
			int x = getIdxRow(src);
			int y = getIdxCol(src);

			p_bit = preGen.rowMoveTab[y - BOARD_LEFT] + this->bitRow[x];
			// 向右
			dst = p_bit->nonCap[0] + (x << 4); // x << 4 获取当前行首元素下标
			while (dst != src)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = 0;
				dst--;
			}
			// 向左
			dst = p_bit->nonCap[1] + (x << 4);
			while (dst != src)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = 0;
				dst++;
			}
			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// 向下
			dst = p_bit->nonCap[0] + y;
			while (dst != src)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = 0;
				dst -= 16;
			}
			// 向上
			dst = p_bit->nonCap[1] + y;
			while (dst != src)
			{
				moves[cnt].step = getMoveType(src, dst);
				moves[cnt++].value = 0;
				dst += 16;
			}
		}
	}
	// 兵的着法
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
//返回值为0/1/将军的棋子值（多子同时将军时返回1）；simple模式下返回值仅为0/1
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

	// 将帅照面
	dst = this->pieces[opptag + JIANG_FROM];
	if (dst)
		if (y == getIdxCol(dst) && (p_bitcol->ju_cap & preGen.bitColMask[dst]))
		{
			if (simple || record)
				return 48;
			else
				record = opptag + JIANG_FROM;
		}

	// 被马将军
	for (int i = MA_FROM; i <= MA_TO; i++)
	{
		dst = this->pieces[opptag + i];
		if (dst)
		{
			int p_leg = dst + preMaLegTab[src - dst + 256];
			if (p_leg != dst && this->board[p_leg] == 0)	//同时判断能否吃到子+别腿
			{
				if (simple || record)
					return 48;
				else
					record = opptag + i;
			}
		}
	}
	// 被车将军
	for (int i = JU_FROM; i <= JU_TO; i++)
	{
		dst = this->pieces[opptag + i];
		if (dst)
		{
			if (x == getIdxRow(dst) && (p_bitrow->ju_cap & preGen.bitRowMask[dst]))
			{
				if (simple || record)
					return 48;
				else
					record = opptag + i;
			}
			else if (y == getIdxCol(dst) && (p_bitcol->ju_cap & preGen.bitColMask[dst]))
			{
				if (simple || record)
					return 48;
				else
					record = opptag + i;
			}
		}
	}
	// 被炮将军
	for (int i = PAO_FROM; i <= PAO_TO; i++)
	{
		dst = this->pieces[opptag + i];
		if (dst)
		{
			if (x == getIdxRow(dst) && (p_bitrow->pao_cap & preGen.bitRowMask[dst]))
			{
				if (simple || record)
					return 48;
				else
					record = opptag + i;
			}
			else if (y == getIdxCol(dst) && (p_bitcol->pao_cap & preGen.bitColMask[dst]))
			{
				if (simple || record)
					return 48;
				else
					record = opptag + i;
			}
		}
	}
	// 被兵将军，左右两格
	for (dst = src - 1; dst <= src + 1; dst += 2)
	{
		int piece = this->board[dst];
		if (pieceType[piece] == 6 && (piece & opptag))
		{
			if (simple || record)
				return 48;
			else
				for (int i = BING_FROM; i <= BING_TO; i++)
					if (pieces[opptag + i] == dst)
					{
						record = opptag + i;
						break;
					}
		}
	}
	//被兵将军，前方
	{
		int piece = this->board[src - 16 + ((tag / 16 - 1) << 5)];	//需测试
		if (pieceType[piece] == 6 && (piece & opptag))
		{
			if (simple || record)
				return 48;
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

int Game::chasedBy(uint16_t mv)
{
	int x, y;
	int src, dst, tag;
	int pcMoved, pcCaptured;
	int temp = (!cur_player) << 7;
	uint8_t* p, * p_leg;
	SlideMoveStruct* p_bit;
	// 判断时 已经执行了这步移动
	src = getDst(mv);
	pcMoved = this->board[src];
	tag = sideTag(this->cur_player);

	switch (pcMoved - sideTag(!this->cur_player))
	{
		case MA_FROM:
		case MA_TO:
			p = preGen.preMaMoves[src];
			p_leg = preGen.preMaLegs[src];
			dst = *p;
			while (dst)
			{
				if (this->board[*p_leg] == 0)
				{
					pcCaptured = this->board[dst];
					if ((pcCaptured & tag) != 0)
					{
						pcCaptured -= tag;
						if (pcCaptured <= JU_TO)
						{
							if (pcCaptured >= JU_FROM)
								return pcCaptured;
						}
						else
						{
							if (pcCaptured <= PAO_TO)
							{
								// 马捉炮 要判断炮是否受到保护
								if (!isProtected(tag, dst))
									return pcCaptured;
							}
							else
								// 马捉兵 要判断兵是否过河且受到保护
								if (((dst & 0x80) == (temp)) && !isProtected(tag, dst))
									return pcCaptured;
						}
					}
				}
				p++;
				p_leg++;
				dst = *p;
			}
			break;
		case JU_FROM:
		case JU_TO:
			x = getIdxCol(src);
			y = getIdxRow(src);
			if (((getSrc(mv) ^ src) & 0xf) == 0)
			{
				p_bit = preGen.rowMoveTab[x - BOARD_LEFT] + this->bitRow[y];
				for (int i = 0; i < 2; i++)
				{
					dst = p_bit->juCap[i] + (y << 4);
					if (dst != src)
					{
						pcCaptured = this->board[dst];
						if (pcCaptured & tag)
						{
							pcCaptured -= tag;
							if (pcCaptured <= JU_TO)
							{
								if (pcCaptured >= MA_FROM && pcCaptured <= MA_TO && !isProtected(tag, dst))
									return pcCaptured;
							}
							else
							{
								if (pcCaptured <= PAO_TO)
								{
									if (!isProtected(tag, dst))
										return pcCaptured;
								}
								else if (((dst & 0x80) == (temp)) && !isProtected(tag, dst))
									return pcCaptured;

							}
						}
					}
				}
			}
			else
			{
				p_bit = preGen.colMoveTab[y - BOARD_TOP] + this->bitCol[x];
				for (int i = 0; i < 2; i++)
				{
					dst = p_bit->juCap[i] + x;
					if (dst != src)
					{
						pcCaptured = this->board[dst];
						if (pcCaptured & tag)
						{
							pcCaptured -= tag;
							if (pcCaptured <= JU_TO)
							{
								if (pcCaptured >= MA_FROM && pcCaptured <= MA_TO && !isProtected(tag, dst))
									return pcCaptured;
							}
							else
							{
								if (pcCaptured <= PAO_TO)
								{
									if (!isProtected(tag, dst))
										return pcCaptured;
								}
								else if (((dst & 0x80) == (temp)) && !isProtected(tag, dst))
									return pcCaptured;

							}
						}
					}
				}
			}
			break;
		case PAO_FROM:
		case PAO_TO:
			x = getIdxCol(src);
			y = getIdxRow(src);
			if (((getSrc(mv) ^ src) & 0xf) == 0)
			{
				p_bit = preGen.rowMoveTab[x - BOARD_LEFT] + this->bitRow[y];
				for (int i = 0; i < 2; i++)
				{
					dst = p_bit->paoCap[i] + (y << 4);
					if (dst != src)
					{
						pcCaptured = this->board[dst];
						if (pcCaptured & tag)
						{
							pcCaptured -= tag;
							if (pcCaptured <= JU_TO)
							{
								if (pcCaptured >= MA_FROM)
								{
									if (pcCaptured <= MA_TO)
									{
										if (!isProtected(tag, dst))
											return pcCaptured;
									}
									else
										return pcCaptured;
								}
							}
							else
							{
								if (pcCaptured >= BING_FROM && ((dst & 0x80) == (temp)) && !isProtected(tag, dst))
									return pcCaptured;

							}
						}
					}
				}
			}
			else
			{
				p_bit = preGen.colMoveTab[y - BOARD_TOP] + this->bitCol[x];
				for (int i = 0; i < 2; i++)
				{
					dst = p_bit->paoCap[i] + x;
					if (dst != src)
					{
						pcCaptured = this->board[dst];
						if (pcCaptured & tag)
						{
							pcCaptured -= tag;
							if (pcCaptured <= JU_TO)
							{
								if (pcCaptured >= MA_FROM)
								{
									if (pcCaptured <= MA_TO)
									{
										if (!isProtected(tag, dst))
											return pcCaptured;
									}
									else
										return pcCaptured;
								}
							}
							else
							{
								if (pcCaptured >= BING_FROM && ((dst & 0x80) == (temp)) && !isProtected(tag, dst))
									return pcCaptured;

							}
						}
					}
				}
			}
			break;
		default:
			break;
	}

	return 0;
}
