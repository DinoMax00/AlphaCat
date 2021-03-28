#include <iostream>
#include <string>

#include "move.h"
#include "base.h"
#include "board.h"

Move::Move()
{
}

std::string Move::moveToString()
{
	
	std::string move = "";
	move += char((from & 15) - 4 + 'a');
	move += char(12 - (from >> 4) + '0');
	move += char((to & 15) - 4 + 'a');
	move += char(12 - (to >> 4) + '0');
	return move;
}

Move::Move(int from, int to)
{
	this->from = from;
	this->to = to;
}


Move::Move(std::string move)
{
	from = ((12 + '0' - move[1]) << 4) + move[0] - 'a' + 4;
	to = ((12 + '0' - move[3]) << 4) + move[2] - 'a' + 4;
}

void Board::generateMoves()
{
	mov.clear();
	for (int index = 0x34; index <= 0xcc; index++)
	{
		switch (board[index])
		{
			case R_JU:	//车
			{
				if (player == BLACK)
					break;
				for (int temp = index + 0x10; board[temp] != 255; temp += 0x10)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
				if (player == RED)
					break;
				for (int temp = index + 0x10; board[temp] != 255; temp += 0x10)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
				if (player == BLACK)
					break;
				for (int i = 0; i < 8; i++)
				{
					if (board[index + MA_Feasible[i]] != 255 && (!board[index + MA_Feasible[i]] || board[index + MA_Feasible[i]] >= 100) && !board[index + MA_Leg[i]])
					{
						Move Curstep(index, index + MA_Feasible[i]);	//需存储
						mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case B_MA:
			{
				if (player == RED)
					break;
				for (int i = 0; i < 8; i++)
				{
					if (board[index + MA_Feasible[i]] != 255 && board[index + MA_Feasible[i]] <= 100 && !board[index + MA_Leg[i]])
					{
						Move Curstep(index, index + MA_Feasible[i]);	//需存储
						mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case R_XIANG:
			{
				if (player == BLACK)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + XIANG_Feasible[i]] != 255 && (!board[index + XIANG_Feasible[i]] || board[index + XIANG_Feasible[i]] >= 100)
						&& !board[index + (XIANG_Feasible[i] / 2)] && ((index + XIANG_Feasible[i]) & 0x80))
					{
						Move Curstep(index, index + XIANG_Feasible[i]);	//需存储
						mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case B_XIANG:
			{
				if (player == RED)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + XIANG_Feasible[i]] != 255 && board[index + XIANG_Feasible[i]] <= 100 && !board[index + (XIANG_Feasible[i] / 2)] && !((index + XIANG_Feasible[i]) & 0x80))
					{
						Move Curstep(index, index + XIANG_Feasible[i]);	//需存储
						mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case R_SHI:
			{
				if (player == BLACK)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + SHI_Feasible[i]] != 255 && (!board[index + SHI_Feasible[i]] || board[index + SHI_Feasible[i]] >= 100)
						&& abs((index + SHI_Feasible[i]) % 16 - 8) <= 1 && abs((index + SHI_Feasible[i]) / 16 - 11) <= 1)
					{
						Move Curstep(index, index + SHI_Feasible[i]);	//需存储
						mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case B_SHI:
			{
				if (player == RED)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + SHI_Feasible[i]] != 255 && board[index + SHI_Feasible[i]] <= 100 && (index + SHI_Feasible[i] % 16 >= 7)
						&& abs((index + SHI_Feasible[i]) % 16 - 8) <= 1 && abs((index + SHI_Feasible[i]) / 16 - 4) <= 1)
					{
						Move Curstep(index, index + SHI_Feasible[i]);	//需存储
						mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case R_JIANG:
			{
				if (player == BLACK)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + JIANG_Feasible[i]] != 255 && (!board[index + JIANG_Feasible[i]] || board[index + JIANG_Feasible[i]] >= 100)
						&& abs((index + JIANG_Feasible[i]) % 16 - 8) <= 1 && abs((index + JIANG_Feasible[i]) / 16 - 11) <= 1)
					{
						Move Curstep(index, index + JIANG_Feasible[i]);	//需存储
						mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case B_JIANG:
			{
				if (player == RED)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (board[index + JIANG_Feasible[i]] != 255 && board[index + JIANG_Feasible[i]] <= 100
						&& abs((index + JIANG_Feasible[i]) % 16 - 8) <= 1 && abs((index + JIANG_Feasible[i]) / 16 - 4) <= 1)
					{
						Move Curstep(index, index + JIANG_Feasible[i]);	//需存储
						mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case R_PAO:				//炮比较麻烦
			{
				if (player == BLACK)
					break;
				bool flag = false;
				for (int temp = index + 0x10; board[temp] != 255; temp += 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x10; board[temp] != 255; temp -= 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index + 0x01; board[temp] != 255; temp += 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x01; board[temp] != 255; temp -= 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
						break;
					}
				}
				break;
			}
			case B_PAO:				//炮比较麻烦
			{
				if (player == RED)
					break;
				bool flag = false;
				for (int temp = index + 0x10; board[temp] != 255; temp += 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x10; board[temp] != 255; temp -= 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index + 0x01; board[temp] != 255; temp += 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x01; board[temp] != 255; temp -= 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.emplace_back(Curstep);
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
						mov.emplace_back(Curstep);
						break;
					}
				}
				break;
			}
			case R_BING:
			{
				if (player == BLACK)
					break;
				if (!(index & 0x80))	//已过河
				{
					for (int i = 0; i < 3; i++)
					{
						if (board[index + BING_R_Feasible[i]] != 255 && (!board[index + BING_R_Feasible[i]] || board[index + BING_R_Feasible[i]] >= 100))
						{
							Move Curstep(index, index + BING_R_Feasible[i]);	//需存储
							mov.emplace_back(Curstep);
						}
					}
				}
				else
				{
					if (board[index - 0x10] != 255 && (!board[index - 0x10] || board[index - 0x10] >= 100))
					{
						Move Curstep(index, index - 0x10);	//需存储
						mov.emplace_back(Curstep);
					}
				}
				break;
			}
			case B_BING:
			{
				if (player == RED)
					break;
				if (index & 0x80)	//已过河
				{
					for (int i = 0; i < 3; i++)
					{
						if (board[index + BING_B_Feasible[i]] != 255 && board[index + BING_B_Feasible[i]] <= 100)
						{
							Move Curstep(index, index + BING_B_Feasible[i]);	//需存储
							mov.emplace_back(Curstep);
						}
					}
				}
				else
				{
					if (board[index + 0x10] != 255 && board[index + 0x10] <= 100)
					{
						Move Curstep(index, index + 0x10);	//需存储
						mov.emplace_back(Curstep);
					}
				}
				break;
			}
		}
	}
	updMovs();
}