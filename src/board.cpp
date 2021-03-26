#include <iostream>

#include"board.h"
#include"base.h"

Board::Board() {}

Board::Board(std::string fen)
{
	buildBoardFromFen(fen);
}


void Board::buildBoardFromFen(std::string fen)
{
	for (int i = 0; i < 256; i++) 
	{
		board[i] = BOUNDARY;
	}
	int position_now = 0x34;
	for (auto i : fen) 
	{
		int empty = i - '0';
		if (0 < empty && empty <= 9) 
		{
			for (int j = 0; j < empty; j++)
				board[position_now++] = EMPTY;
		}
		else if (i == '/') position_now += 7;//跳到下一行的第一位
		else if (i == 'r') board[position_now++] = B_JU;
		else if (i == 'n') board[position_now++] = B_MA;
		else if (i == 'b') board[position_now++] = B_XIANG;
		else if (i == 'a') board[position_now++] = B_SHI;
		else if (i == 'k') board[position_now++] = B_JIANG;
		else if (i == 'c') board[position_now++] = B_PAO;
		else if (i == 'p') board[position_now++] = B_BING;
		else if (i == 'R') board[position_now++] = R_JU;
		else if (i == 'N') board[position_now++] = R_MA;
		else if (i == 'B') board[position_now++] = R_XIANG;
		else if (i == 'A') board[position_now++] = R_SHI;
		else if (i == 'K') board[position_now++] = R_JIANG;
		else if (i == 'C') board[position_now++] = R_PAO;
		else if (i == 'P') board[position_now++] = R_BING;
	}
}

void Board::genOneMove(std::string move)
{
	unsigned char start_position = ((12 - move[1] - '0') << 4) + move[0] - 'a' + 4;
	unsigned char end_position = ((12 - move[3] - '0') << 4) + move[2] - 'a' + 4;
	board[end_position] = board[start_position];
	board[start_position] = EMPTY;
}

void Board::printBoardForDebug() 
{
	for (int i = 0; i < 256; i++) 
	{
		if ((i >> 4) >= 3 && (i >> 4) <= 12 && (i & 15) == 2) //打印列标
		{
			std::cout << 12 - ((i >> 4)) << " ";
			continue;
		}
		if ((i >> 4) == 14 && (i & 15) >= 4 && (i & 15) <= 12) //打印行标
		{
			std::cout << char((i & 15) - 4 + 'a') << " ";
			continue;
		}
		switch (board[i])
		{
			case BOUNDARY:
				std::cout << "  ";
				break;
			case B_JU:
				std::cout << "r ";
				break;
			case B_MA:
				std::cout << "n ";
				break;
			case B_XIANG:
				std::cout << "b ";
				break;
			case B_SHI:
				std::cout << "a ";
				break;
			case B_JIANG:
				std::cout << "k ";
				break;
			case B_PAO:
				std::cout << "c ";
				break;
			case B_BING:
				std::cout << "p ";
				break;
			case R_JU:
				std::cout << "R ";
				break;
			case R_MA:
				std::cout << "N ";
				break;
			case R_XIANG:
				std::cout << "B ";
				break;
			case R_SHI:
				std::cout << "A ";
				break;
			case R_JIANG:
				std::cout << "K ";
				break;
			case R_PAO:
				std::cout << "C ";
				break;
			case R_BING:
				std::cout << "P ";
				break;
			case EMPTY:
				std::cout << ". ";
				break;
			default:
				std::cout << "? ";
				break;
		}
		if (i % 16 == 15) std::cout << std::endl;
	}
}



///////////////////////


bool Board::judgeBorder(unsigned char dest)
{
	if (board[dest] == 255)
		return false;
	return true;
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