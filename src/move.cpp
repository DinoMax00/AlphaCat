#include <iostream>
#include <string>

#include "move.h"

std::string Move::moveToString()
{
	std::string move = "";
	move += char((from & 15) - 4 + 'a');
	// std::cout << ((from & 15) - 4 + 'a') << std::endl;
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

void Board::generateMoves(bool isRed)
{
	for (int index = 0x34; index <= 0xcc; index++)
	{
		switch (board[index])
		{
			case 1:	//车
			{
				if (!isRed)
					break;
				for (int temp = index + 0x10; judgeBorder(temp); temp += 0x10)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index - 0x10; judgeBorder(temp); temp -= 0x10)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index + 0x01; judgeBorder(temp); temp += 0x01)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index - 0x01; judgeBorder(temp); temp -= 0x01)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				break;
			}
			case 101:
			{
				if (isRed)
					break;
				for (int temp = index + 0x10; judgeBorder(temp); temp += 0x10)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index - 0x10; judgeBorder(temp); temp -= 0x10)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index + 0x01; judgeBorder(temp); temp += 0x01)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				for (int temp = index - 0x01; judgeBorder(temp); temp -= 0x01)
				{
					if (!board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
						break;
					}
					else
					{
						break;
					}
				}
				break;
			}
			case 2:
			{
				if (!isRed)
					break;
				for (int i = 0; i < 8; i++)
				{
					if (judgeBorder(index + MA_Feasible[i]) && (!board[index + MA_Feasible[i]] || board[index + MA_Feasible[i]] >= 100) && !board[index + MA_Leg[i]])
					{
						Move Curstep(index, index + MA_Feasible[i]);	//需存储
						mov.push_back(Curstep);
					}
				}
				break;
			}
			case 102:
			{
				if (isRed)
					break;
				for (int i = 0; i < 8; i++)
				{
					if (judgeBorder(index + MA_Feasible[i]) && board[index + MA_Feasible[i]] <= 100 && !board[index + MA_Leg[i]])
					{
						Move Curstep(index, index + MA_Feasible[i]);	//需存储
						mov.push_back(Curstep);
					}
				}
				break;
			}
			case 3:
			{
				if (!isRed)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (judgeBorder(index + XIANG_Feasible[i]) && (!board[index + XIANG_Feasible[i]] || board[index + XIANG_Feasible[i]] >= 100)
						&& !board[index + XIANG_Feasible[i] / 2] && ((index + XIANG_Feasible[i]) & 0x80))
					{
						Move Curstep(index, index + XIANG_Feasible[i]);	//需存储
						mov.push_back(Curstep);
					}
				}
				break;
			}
			case 103:
			{
				if (isRed)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (judgeBorder(index + XIANG_Feasible[i]) && board[index + XIANG_Feasible[i]] <= 100 && !board[index + XIANG_Feasible[i] / 2] && !((index + XIANG_Feasible[i]) & 0x80))
					{
						Move Curstep(index, index + XIANG_Feasible[i]);	//需存储
						mov.push_back(Curstep);
					}
				}
				break;
			}
			case 4:
			{
				if (!isRed)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (judgeBorder(index + SHI_Feasible[i]) && (!board[index + SHI_Feasible[i]] || board[index + SHI_Feasible[i]] >= 100)
						&& abs((index + SHI_Feasible[i]) % 16 - 8) + abs((index + SHI_Feasible[i]) / 16 - 11) <= 1)
					{
						Move Curstep(index, index + SHI_Feasible[i]);	//需存储
						mov.push_back(Curstep);
					}
				}
				break;
			}
			case 104:
			{
				if (isRed)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (judgeBorder(index + SHI_Feasible[i]) && board[index + SHI_Feasible[i]] <= 100 && (index + SHI_Feasible[i] % 16 >= 7)
						&& (abs((index + SHI_Feasible[i]) % 16 - 8) + abs((index + SHI_Feasible[i]) / 16 - 4)) <= 1)
					{
						Move Curstep(index, index + SHI_Feasible[i]);	//需存储
						mov.push_back(Curstep);
					}
				}
				break;
			}
			case 5:
			{
				if (!isRed)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (judgeBorder(index + JIANG_Feasible[i]) && (!board[index + JIANG_Feasible[i]] || board[index + JIANG_Feasible[i]] >= 100))
					{
						Move Curstep(index, index + JIANG_Feasible[i]);	//需存储
						mov.push_back(Curstep);
					}
				}
				break;
			}
			case 105:
			{
				if (isRed)
					break;
				for (int i = 0; i < 4; i++)
				{
					if (judgeBorder(index + JIANG_Feasible[i]) && board[index + JIANG_Feasible[i]] <= 100)
					{
						Move Curstep(index, index + JIANG_Feasible[i]);	//需存储
						mov.push_back(Curstep);
					}
				}
				break;
			}
			case 6:				//炮比较麻烦
			{
				if (!isRed)
					break;
				bool flag = false;
				for (int temp = index + 0x10; judgeBorder(temp); temp += 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
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
						mov.push_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x10; judgeBorder(temp); temp -= 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
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
						mov.push_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index + 0x01; judgeBorder(temp); temp += 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
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
						mov.push_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x01; judgeBorder(temp); temp -= 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
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
						mov.push_back(Curstep);
						break;
					}
				}
				break;
			}
			case 106:				//炮比较麻烦
			{
				if (isRed)
					break;
				bool flag = false;
				for (int temp = index + 0x10; judgeBorder(temp); temp += 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
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
						mov.push_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x10; judgeBorder(temp); temp -= 0x10)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
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
						mov.push_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index + 0x01; judgeBorder(temp); temp += 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
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
						mov.push_back(Curstep);
						break;
					}
				}
				flag = false;
				for (int temp = index - 0x01; judgeBorder(temp); temp -= 0x01)
				{
					if (!flag && !board[temp])
					{
						Move Curstep(index, temp);
						mov.push_back(Curstep);
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
						mov.push_back(Curstep);
						break;
					}
				}
				break;
			}
			case 7:
			{
				if (!isRed)
					break;
				if (!(index & 0x80))	//已过河
				{
					for (int i = 0; i < 3; i++)
					{
						if (judgeBorder(index + BING_R_Feasible[i]) && (!board[index + BING_R_Feasible[i]] || board[index + BING_R_Feasible[i]] >= 100))
						{
							Move Curstep(index, index + BING_R_Feasible[i]);	//需存储
							mov.push_back(Curstep);
						}
					}
				}
				else
				{
					if (judgeBorder(index - 0x10) && (!board[index - 0x10] || board[index - 0x10] >= 100))
					{
						Move Curstep(index, index - 0x10);	//需存储
						mov.push_back(Curstep);
					}
				}
				break;
			}
			case 107:
			{
				if (isRed)
					break;
				if (index & 0x80)	//已过河
				{
					for (int i = 0; i < 3; i++)
					{
						if (judgeBorder(index + BING_B_Feasible[i]) && board[index + BING_B_Feasible[i]] <= 100)
						{
							Move Curstep(index, index + BING_B_Feasible[i]);	//需存储
							mov.push_back(Curstep);
						}
					}
				}
				else
				{
					if (judgeBorder(index + 0x10) && board[index + 0x10] <= 100)
					{
						Move Curstep(index, index + 0x10);	//需存储
						mov.push_back(Curstep);
					}
				}
				break;
			}
		}
	}
}