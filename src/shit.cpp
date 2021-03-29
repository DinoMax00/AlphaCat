#include <iostream>
#include <random>
#include <chrono>

#include "board.h"
#include "base.h"
#include "log.h"


bool Board::checkJiang(Move& mov)
{
	return checkJiang(mov, player);
}

bool Board::checkJiang(Move& mov, bool side)
{
	genOneMove(mov);
	bool flag = 0;

	//判断是否有将帅直面
	if ((pos_of_kings[0] & 15) == (pos_of_kings[1] & 15))
	{
		flag = 1;
		int mn = std::min(pos_of_kings[0], pos_of_kings[1]), mx = std::max(pos_of_kings[0], pos_of_kings[1]);
		for (int i = mn + 16; i < mx; i += 16)
			if (board[i] != EMPTY)
				flag = 0;
	}
	if (flag)
	{
		deleteOneMove(mov);
		return true;
	}

	//向四个方向遍历，是否被兵、車和炮将军
	int cnt = 0; //走直线时路上遇到的子的总个数
	//向下遍历
	for (int i = pos_of_kings[side] + 16; board[i] != BOUNDARY; i += 16)
	{
		if (board[i] == EMPTY) continue;
		else
		{
			//被兵将军
			if (i == pos_of_kings[side] + 16)
			{
				if ((side == BLACK && board[i] == R_BING) || (side == RED && board[i] == B_BING))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被車将军
			if (cnt == 0)
			{
				if ((side == BLACK && board[i] == R_JU) || (side == RED && board[i] == B_JU))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被炮将军
			if (cnt == 1)
			{
				if ((side == BLACK && board[i] == R_PAO) || (side == RED && board[i] == B_PAO))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			++cnt;
		}
		if (cnt > 1) break;
	}

	cnt = 0;
	//向上遍历
	for (int i = pos_of_kings[side] - 16; board[i] != BOUNDARY; i -= 16)
	{
		if (board[i] == EMPTY) continue;
		else
		{
			//被兵将军
			if (i == pos_of_kings[side] - 16)
			{
				if ((side == BLACK && board[i] == R_BING) || (side == RED && board[i] == B_BING))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被車将军
			if (cnt == 0)
			{
				if ((side == BLACK && board[i] == R_JU) || (side == RED && board[i] == B_JU))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被炮将军
			if (cnt == 1)
			{
				if ((side == BLACK && board[i] == R_PAO) || (side == RED && board[i] == B_PAO))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			++cnt;
		}
		if (cnt > 1) break;
	}

	cnt = 0;
	//向左遍历
	for (int i = pos_of_kings[side] - 1; board[i] != BOUNDARY; i--)
	{
		if (board[i] == EMPTY) continue;
		else
		{
			//被兵将军
			if (i == pos_of_kings[side] - 1)
			{
				if ((side == BLACK && board[i] == R_BING) || (side == RED && board[i] == B_BING))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被車将军
			if (cnt == 0)
			{
				if ((side == BLACK && board[i] == R_JU) || (side == RED && board[i] == B_JU))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被炮将军
			if (cnt == 1)
			{
				if ((side == BLACK && board[i] == R_PAO) || (side == RED && board[i] == B_PAO))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			++cnt;
		}
		if (cnt > 1) break;
	}

	cnt = 0;
	//向右遍历
	for (int i = pos_of_kings[side] + 1; board[i] != BOUNDARY; i++)
	{
		if (board[i] == EMPTY) continue;
		else
		{
			//被兵将军
			if (i == pos_of_kings[side] + 1)
			{
				if ((side == BLACK && board[i] == R_BING) || (side == RED && board[i] == B_BING))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被車将军
			if (cnt == 0)
			{
				if ((side == BLACK && board[i] == R_JU) || (side == RED && board[i] == B_JU))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被炮将军
			if (cnt == 1)
			{
				if ((side == BLACK && board[i] == R_PAO) || (side == RED && board[i] == B_PAO))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			++cnt;
		}
		if (cnt > 1) break;
	}

	//被马将军
	for (int i = 0; i < 8; i++)
	{
		int to = pos_of_kings[side] + MA_Feasible[i];
		int leg = to - MA_Leg[i];
		if (board[leg] == EMPTY)
		{
			if ((side == BLACK && board[to] == R_MA) || (side == RED && board[to] == B_MA))
			{
				deleteOneMove(mov);
				return true;
			}
		}
	}
	deleteOneMove(mov);
	return false;
}

void Board::generateMoves()
{
	generateMoves(player);
}

void Board::generateMoves(bool side)
{
	mov.clear();
	for (int index = 0x34; index <= 0xcc; index++)
	{
		switch (board[index])
		{
			case R_JU:	//车
			{
				if (side == BLACK)
					break;
				for (int temp = index + 0x10; board[temp] != 255; temp += 0x10)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index - 0x10; board[temp] != 255; temp -= 0x10)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index + 0x01; board[temp] != 255; temp += 0x01)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index - 0x01; board[temp] != 255; temp -= 0x01)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				break;
			}
			case B_JU:
			{
				if (side == RED)
					break;
				for (int temp = index + 0x10; board[temp] != 255; temp += 0x10)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index - 0x10; board[temp] != 255; temp -= 0x10)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index + 0x01; board[temp] != 255; temp += 0x01)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index - 0x01; board[temp] != 255; temp -= 0x01)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				break;
			}
			case R_MA:
			{
				if (side == BLACK)
					break;
				for (int i = 0; i < 8; i++)
				{
					if (board[index + MA_Feasible[i]] != 255 && (!board[index + MA_Feasible[i]] || board[index + MA_Feasible[i]] >= 100) && !board[index + MA_Leg[i]])
					{
						Move Curstep(index, index + MA_Feasible[i]);	//需存储
						Curstep.chessOnTo = board[index + MA_Feasible[i]];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case B_MA:
			{
				if (side == RED)
					break;
				for (int i = 0; i < 8; i++)
				{
					if (board[index + MA_Feasible[i]] != 255 && board[index + MA_Feasible[i]] <= 100 && !board[index + MA_Leg[i]])
					{
						Move Curstep(index, index + MA_Feasible[i]);	//需存储
						Curstep.chessOnTo = board[index + MA_Feasible[i]];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case R_XIANG:
			{
				if (side == BLACK)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + XIANG_Feasible[i]] != 255 && (!board[index + XIANG_Feasible[i]] || board[index + XIANG_Feasible[i]] >= 100)
						&& !board[index + (XIANG_Feasible[i] / 2)] && ((index + XIANG_Feasible[i]) & 0x80))
					{
						Move Curstep(index, index + XIANG_Feasible[i]);	//需存储
						Curstep.chessOnTo = board[index + XIANG_Feasible[i]];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case B_XIANG:
			{
				if (side == RED)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + XIANG_Feasible[i]] != 255 && board[index + XIANG_Feasible[i]] <= 100 && !board[index + (XIANG_Feasible[i] / 2)] && !((index + XIANG_Feasible[i]) & 0x80))
					{
						Move Curstep(index, index + XIANG_Feasible[i]);	//需存储
						Curstep.chessOnTo = board[index + XIANG_Feasible[i]];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case R_SHI:
			{
				if (side == BLACK)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + SHI_Feasible[i]] != 255 && (!board[index + SHI_Feasible[i]] || board[index + SHI_Feasible[i]] >= 100)
						&& abs((index + SHI_Feasible[i]) % 16 - 8) <= 1 && abs((index + SHI_Feasible[i]) / 16 - 11) <= 1)
					{
						Move Curstep(index, index + SHI_Feasible[i]);	//需存储
						Curstep.chessOnTo = board[index + SHI_Feasible[i]];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case B_SHI:
			{
				if (side == RED)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + SHI_Feasible[i]] != 255 && board[index + SHI_Feasible[i]] <= 100 && (index + SHI_Feasible[i] % 16 >= 7)
						&& abs((index + SHI_Feasible[i]) % 16 - 8) <= 1 && abs((index + SHI_Feasible[i]) / 16 - 4) <= 1)
					{
						Move Curstep(index, index + SHI_Feasible[i]);	//需存储
						Curstep.chessOnTo = board[index + SHI_Feasible[i]];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case R_JIANG:
			{
				if (side == BLACK)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + JIANG_Feasible[i]] != 255 && (!board[index + JIANG_Feasible[i]] || board[index + JIANG_Feasible[i]] >= 100)
						&& abs((index + JIANG_Feasible[i]) % 16 - 8) <= 1 && abs((index + JIANG_Feasible[i]) / 16 - 11) <= 1)
					{
						Move Curstep(index, index + JIANG_Feasible[i]);	//需存储
						Curstep.chessOnTo = board[index + JIANG_Feasible[i]];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case B_JIANG:
			{
				if (side == RED)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + JIANG_Feasible[i]] != 255 && board[index + JIANG_Feasible[i]] <= 100
						&& abs((index + JIANG_Feasible[i]) % 16 - 8) <= 1 && abs((index + JIANG_Feasible[i]) / 16 - 4) <= 1)
					{
						Move Curstep(index, index + JIANG_Feasible[i]);	//需存储
						Curstep.chessOnTo = board[index + JIANG_Feasible[i]];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case R_PAO:				//炮比较麻烦
			{
				if (side == BLACK)
					break;
				bool flag = false;
				for (int temp = index + 0x10; board[temp] != 255; temp += 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (!flag && board[temp] < 255)
					{
						flag = true;
						continue;
					}
					else if (flag && board[temp] && board[temp] < 100)
					{
						break;
					}
					else if (flag && board[temp] >= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x10; board[temp] != 255; temp -= 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (!flag && board[temp] < 255)
					{
						flag = true;
						continue;
					}
					else if (flag && board[temp] && board[temp] < 100)
					{
						break;
					}
					else if (flag && board[temp] >= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index + 0x01; board[temp] != 255; temp += 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (!flag && board[temp] < 255)
					{
						flag = true;
						continue;
					}
					else if (flag && board[temp] && board[temp] < 100)
					{
						break;
					}
					else if (flag && board[temp] >= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x01; board[temp] != 255; temp -= 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (!flag && board[temp] < 255)
					{
						flag = true;
						continue;
					}
					else if (flag && board[temp] && board[temp] < 100)
					{
						break;
					}
					else if (flag && board[temp] >= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
				}
				break;
			}
			case B_PAO:				//炮比较麻烦
			{
				if (side == RED)
					break;
				bool flag = false;
				for (int temp = index + 0x10; board[temp] != 255; temp += 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (!flag && board[temp] < 255)
					{
						flag = true;
						continue;
					}
					else if (flag && board[temp] >= 100)
					{
						break;
					}
					else if (flag && board[temp] && board[temp] < 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x10; board[temp] != 255; temp -= 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (!flag && board[temp] < 255)
					{
						flag = true;
						continue;
					}
					else if (flag && board[temp] >= 100)
					{
						break;
					}
					else if (flag && board[temp] && board[temp] < 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index + 0x01; board[temp] != 255; temp += 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (!flag && board[temp] < 255)
					{
						flag = true;
						continue;
					}
					else if (flag && board[temp] >= 100)
					{
						break;
					}
					else if (flag && board[temp] && board[temp] < 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x01; board[temp] != 255; temp -= 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
					else if (!flag && board[temp] < 255)
					{
						flag = true;
						continue;
					}
					else if (flag && board[temp] >= 100)
					{
						break;
					}
					else if (flag && board[temp] && board[temp] < 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						break;
					}
				}
				break;
			}
			case R_BING:
			{
				if (side == BLACK)
					break;
				if (!(index & 0x80))	//已过河
				{
					for (int i = 0; i < 3; i++)
					{
						if (board[index + BING_R_Feasible[i]] != 255 && (!board[index + BING_R_Feasible[i]] || board[index + BING_R_Feasible[i]] >= 100))
						{
							Move Curstep(index, index + BING_R_Feasible[i]);	//需存储
							Curstep.chessOnTo = board[index + BING_R_Feasible[i]];
							if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						}
					}
				}
				else
				{
					if (board[index - 0x10] != 255 && (!board[index - 0x10] || board[index - 0x10] >= 100))
					{
						Move Curstep(index, index - 0x10);	//需存储
						Curstep.chessOnTo = board[index - 0x10];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case B_BING:
			{
				if (side == RED)
					break;
				if (index & 0x80)	//已过河
				{
					for (int i = 0; i < 3; i++)
					{
						if (board[index + BING_B_Feasible[i]] != 255 && board[index + BING_B_Feasible[i]] <= 100)
						{
							Move Curstep(index, index + BING_B_Feasible[i]);	//需存储
							Curstep.chessOnTo = board[index + BING_B_Feasible[i]];
							if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
						}
					}
				}
				else
				{
					if (board[index + 0x10] != 255 && board[index + 0x10] <= 100)
					{
						Move Curstep(index, index + 0x10);	//需存储
						Curstep.chessOnTo = board[index + 0x10];
						if (!checkJiang(Curstep, side)) mov.emplace_back(Curstep);
					}
				}
				break;
			}
		}
	}
}