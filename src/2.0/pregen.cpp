#include "pregen.h"

// 位置是否在棋盘中
inline bool inBoard(int pos)
{
	if (pos <= 0x33 || pos >= 0xcb)
		return false;
	pos &= 15;
	return pos >= 0x03 && pos <= 0x0b;
}

// 将 士 在家里
inline bool inHome(int pos)
{
	if (0x36 <= pos && pos <= 0x38)
		return true;
	if (0x46 <= pos && pos <= 0x48)
		return true;
	if (0x56 <= pos && pos <= 0x58)
		return true;
	if (0xa6 <= pos && pos <= 0xa8)
		return true;
	if (0xb6 <= pos && pos <= 0xb8)
		return true;
	if (0xc6 <= pos && pos <= 0xc8)
		return true;
	return false;
}

// 两个位置再同一边
inline bool sameSide(int src, int dst)
{
	return ((src ^ dst) & 0x80) == 0;
}

// 兵向前走一步 区分红黑
inline int bingMoveForward(int pos, int side)
{
	side = !side;
	return pos - 16 + (side << 5);
}

// 在棋盘的半边
inline bool inHalfBoard(int pos, int side)
{
	side = !side;
	return (pos & 0x80) == (side << 7);
}

Pregen preGen;

void preGenInit()
{
	int src, dst, cnt = 0;
	// 着法预生成数组
	for (src = 0; src < 256; src++)
	{
		if (!inBoard(src))
			continue;

		// 将
		cnt = 0;
		for (int i = 0; i < 4; i++)
		{
			dst = src + preJiangTab[i];
			if (inHome(dst))
			{
				preGen.preJiangMoves[src][cnt++] = dst;
			}
		}
		preGen.preJiangMoves[src][cnt] = 0;
		
		// 士
		cnt = 0;
		for (int i = 0; i < 4; i++) 
		{
			dst = src + preShiTab[i];
			if (inHome(dst))
			{
				preGen.preShiMoves[src][cnt++] = dst;
			}
		}
		preGen.preShiMoves[src][cnt] = 0;

		// 相
		cnt = 0;
		for (int i = 0; i < 4; i++)
		{
			dst = src + preXiangTab[i];
			if (inBoard(dst) && sameSide(src, dst))
			{
				preGen.preXiangMoves[src][cnt] = dst;
				preGen.preXiangLegs[src][cnt++] = (src + dst) >> 1;
			}
		}
		preGen.preXiangMoves[src][cnt] = 0;

		// 马
		cnt = 0;
		for (int i = 0; i < 8; i++)
		{
			dst = src + preMaTab[i];
			if(inBoard(dst))
			{
				preGen.preMaMoves[src][cnt] = dst;
				preGen.preMaLegs[src][cnt++] = src + preMaLegTab[dst - src + 256];
			}
		}
		preGen.preMaMoves[src][cnt] = 0;

		// 兵
		for (int i = 0; i < 2; i++)
		{
			cnt = 0;
			// 向前
			dst = src + (i ? -16: 16);
			if (inBoard(dst))
			{
				preGen.preBingMoves[i][src][cnt++] = dst;
			}
			// 左右
			if (inHalfBoard(src, i))
			{
				for (int j = -1; j <= 1; j += 2)
				{
					dst = src + j;
					if (inBoard(dst))
					{
						preGen.preBingMoves[i][src][cnt++] = dst;
					}
				}
			}
			preGen.preBingMoves[i][src][cnt] = 0;
		}
	}
}
