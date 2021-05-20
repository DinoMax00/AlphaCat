#include "pregen.h"
#include "game.h"

// λ���Ƿ���������
inline bool inBoard(int pos)
{
	if (pos < 0x33 || pos > 0xcb)
		return false;
	pos &= 15;
	return pos >= 0x03 && pos <= 0x0b;
}

// �� ʿ �ڼ���
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

// ����λ����ͬһ��
inline bool sameSide(int src, int dst)
{
	return ((src ^ dst) & 0x80) == 0;
}

// ����ǰ��һ�� ���ֺ��
inline int bingMoveForward(int pos, int side)
{
	side = !side;
	return pos - 16 + (side << 5);
}

// �����̵İ��
inline bool inHalfBoard(int pos, int side)
{
	side = !side;
	return (pos & 0x80) == (side << 7);
}

Pregen preGen;

void preGenInit()
{
	SlideMoveStruct move_temp;
	SlideMaskStruct mask_temp;
	int src, dst, cnt = 0, k;
	// ��ʼ������λ��������λ��
	for (src = 0; src < 256; src++)
	{
		if (inBoard(src))
		{
			preGen.bitRowMask[src] = 1 << (getIdxCol(src) - BOARD_LEFT);
			preGen.bitColMask[src] = 1 << (getIdxRow(src) - BOARD_TOP);
		}
		else
		{
			preGen.bitRowMask[src] = 0;
			preGen.bitColMask[src] = 0;
		}
	}

	// �� �� ����
	for (int i = 0; i < 9; i++)
	{
		// ö��2^9��������Ϣ
		for (int j = 0; j < 512; j++)
		{
			move_temp.nonCap[0] = move_temp.nonCap[1] =
				move_temp.juCap[0] = move_temp.juCap[1] =
				move_temp.paoCap[0] = move_temp.paoCap[1] =
				move_temp.cannonCap[0] = move_temp.cannonCap[1] =
				i + BOARD_LEFT;
			mask_temp = {0, 0, 0, 0};

			// ����
			for (k = i + 1; k <= 8; k++)
			{	
				if (j & (1 << k))
				{
					move_temp.juCap[0] = k + BOARD_LEFT;
					mask_temp.ju_cap |= 1 << k;
					break;
				}
				move_temp.nonCap[0] = k + BOARD_LEFT;
				mask_temp.non_cap |= 1 << k;
			}
			for (k++; k <= 8; k++)
			{
				if (j & (1 << k))
				{
					move_temp.paoCap[0] = k + BOARD_LEFT;
					mask_temp.pao_cap |= 1 << k;
					break;
				}
			}
			for (k++; k <= 8; k++)
			{
				if (j & (1 << k))
				{
					move_temp.cannonCap[0] = k + BOARD_LEFT;
					mask_temp.cannon_cap |= 1 << k;
					break;
				}
			}
			// ����
			for (k = i - 1; k >= 0; k--)
			{
				if (j & (1 << k))
				{
					move_temp.juCap[1] = k + BOARD_LEFT;
					mask_temp.ju_cap |= 1 << k;
					break;
				}
				move_temp.nonCap[1] = k + BOARD_LEFT;
				mask_temp.non_cap |= 1 << k;
			}
			for (k--; k >= 0; k--)
			{
				if (j & (1 << k))
				{
					move_temp.paoCap[1] = k + BOARD_LEFT;
					mask_temp.pao_cap |= 1 << k;
					break;
				}
			}
			for (k--; k >= 0; k--)
			{
				if (j & (1 << k))
				{
					move_temp.cannonCap[1] = k + BOARD_LEFT;
					mask_temp.cannon_cap |= 1 << k;
					break;
				}
			}

			preGen.rowMoveTab[i][j] = move_temp;
			preGen.rowMaskTab[i][j] = mask_temp;
		}
	}

	// �� �� ����
	for (int i = 0; i < 10; i++)
	{
		// ÿ��10�� 2^10
		for (int j = 0; j < 1024; j++)
		{
			move_temp.nonCap[0] = move_temp.nonCap[1] =
				move_temp.juCap[0] = move_temp.juCap[1] =
				move_temp.paoCap[0] = move_temp.paoCap[1] =
				move_temp.cannonCap[0] = move_temp.cannonCap[1] = 
				(i + BOARD_TOP) << 4;
			mask_temp = { 0, 0, 0, 0};

			// ����
			for (k = i + 1; k <= 9; k++)
			{
				if (j & (1 << k))
				{
					move_temp.juCap[0] = (k + BOARD_TOP) << 4;
					mask_temp.ju_cap |= 1 << k;
					break;
				}
				move_temp.nonCap[0] = (k + BOARD_TOP) << 4;
				mask_temp.non_cap |= 1 << k;
			}
			for (k++; k <= 9; k++)
			{
				if (j & (1 << k))
				{
					move_temp.paoCap[0] = (k + BOARD_TOP) << 4;
					mask_temp.pao_cap |= 1 << k;
					break;
				}
			}
			for (k++; k <= 9; k++)
			{
				if (j & (1 << k))
				{
					move_temp.cannonCap[0] = (k + BOARD_TOP) << 4;
					mask_temp.cannon_cap |= 1 << k;
					break;
				}
			}
			// ����
			for (k = i - 1; k >= 0; k--)
			{
				if (j & (1 << k))
				{
					move_temp.juCap[1] = (k + BOARD_TOP) << 4;
					mask_temp.ju_cap |= 1 << k;
					break;
				}
				move_temp.nonCap[1] = (k + BOARD_TOP) << 4;
				mask_temp.non_cap |= 1 << k;
			}
			for (k--; k >=0; k--)
			{
				if (j & (1 << k))
				{
					move_temp.paoCap[1] = (k + BOARD_TOP) << 4;
					mask_temp.pao_cap |= 1 << k;
					break;
				}
			}
			for (k--; k >= 0; k--)
			{
				if (j & (1 << k))
				{
					move_temp.cannonCap[1] = (k + BOARD_TOP) << 4;
					mask_temp.cannon_cap |= 1 << k;
					break;
				}
			}
			preGen.colMoveTab[i][j] = move_temp;
			preGen.colMaskTab[i][j] = mask_temp;
		}
	}

	// �ŷ�Ԥ��������
	for (src = 0; src < 256; src++)
	{
		if (!inBoard(src))
			continue;

		// ��
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
		
		// ʿ
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

		// ��
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

		// ��
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

		// ��
		for (int i = 0; i < 2; i++)
		{
			cnt = 0;
			// ��ǰ
			dst = src + (i ? -16: 16);
			if (inBoard(dst))
			{
				preGen.preBingMoves[i][src][cnt++] = dst;
			}
			// ����
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
