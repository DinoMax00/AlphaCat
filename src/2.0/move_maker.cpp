// 走法生成
#include <iostream>
#include "game.h"
#include "pregen.h"


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
	src = this->pieces[JIANG_FROM + tag];
	if (src)
	{
		p = preGen.preJiangMoves[src];
		while (*p)
		{
			dst = *p;
			// 是否吃子
			if (this->board[dst] & opptag)	//借助16/32位运算判断
				moves[cnt++].step = getMoveType(src, dst);
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
					moves[cnt++].step = getMoveType(src, dst);
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
					moves[cnt++].step = getMoveType(src, dst);
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
					moves[cnt++].step = getMoveType(src, dst);
				p++;
				p_leg++;
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
			dst = p_bit->PaoCap[0] + (x << 4); // x << 4 获取当前行首元素下标
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
			// 向左
			dst = p_bit->PaoCap[1] + (x << 4);
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}

			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// 向下
			dst = p_bit->PaoCap[0] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
			// 向上
			dst = p_bit->PaoCap[1] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
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
			dst = p_bit->JuCap[0] + (x << 4); // x << 4 获取当前行首元素下标
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
			// 向左
			dst = p_bit->JuCap[1] + (x << 4);
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// 向下
			dst = p_bit->JuCap[0] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
			}
			// 向上
			dst = p_bit->JuCap[1] + y;
			if (this->board[dst] & opptag)
			{
				moves[cnt++].step = getMoveType(src, dst);
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
	src = this->pieces[JIANG_FROM + tag];
	if (src)
	{
		p = preGen.preJiangMoves[src];
		while (*p)
		{
			dst = *p;
			// 是否吃子
			if (this->board[dst] == 0)
				moves[cnt++].step = getMoveType(src, dst);
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
					moves[cnt++].step = getMoveType(src, dst);
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
					moves[cnt++].step = getMoveType(src, dst);
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
					moves[cnt++].step = getMoveType(src, dst);
				p++;
				p_leg++;
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
					moves[cnt++].step = getMoveType(src, dst);
				p++;
			}
		}
	}

	// 炮和车的着法 走直线，不作区分
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
			dst = p_bit->NonCap[0] + (x << 4); // x << 4 获取当前行首元素下标
			while (dst != src)
			{
				moves[cnt++].step = getMoveType(src, dst);
				dst--;
			}
			// 向左
			dst = p_bit->NonCap[1] + (x << 4);
			while (dst != src)
			{
				moves[cnt++].step = getMoveType(src, dst);
				dst++;
			}
			p_bit = preGen.colMoveTab[x - BOARD_TOP] + this->bitCol[y];
			// 向下
			dst = p_bit->NonCap[0] + y;
			while (dst != src)
			{
				moves[cnt++].step = getMoveType(src, dst);
				dst -= 16;
			}
			// 向上
			dst = p_bit->NonCap[1] + y;
			while (dst != src)
			{
				moves[cnt++].step = getMoveType(src, dst);
				dst += 16;
			}
		}
	}
	return cnt;
}


bool Game::detectCheck()
//返回值仅为将军判断，后期视需求改动
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

	// 将帅照面
	dst = this->pieces[JIANG_FROM + opptag];
	if (dst)
		if (y == getIdxCol(dst) && (p_bitcol->JuCap & preGen.bitColMask[dst]))
			return true;

	// 被马将军
	for (int i = MA_FROM; i <= MA_TO; i++)
	{
		dst = this->pieces[i + opptag];
		if (dst)
		{
			int p_leg = dst + preMaLegTab[src - dst + 256];
			if (p_leg != dst && this->board[p_leg] == 0)	//同时判断能否吃到子+别腿
				return true;
		}
	}

	// 被兵将军，搜索左右两格
	for (dst = src - 1; dst <= src + 1; dst += 2)
	{
		int piece = this->board[dst];
		if (pieceType[piece] == 6 && (piece & opptag))
			return true;
	}
	//搜索前方
	{
		int piece = this->board[src - 16 + ((tag / 16 - 1) << 5)];	//需测试
		if (pieceType[piece] == 6 && (piece & opptag))
			return true;
	}

	// 被炮将军
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

	// 被车将军
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
	return false;
}