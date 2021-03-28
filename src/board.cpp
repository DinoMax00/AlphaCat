#include <iostream>
#include <random>
#include <chrono>

#include "board.h"
#include "base.h"
#include "log.h"

Board::Board() 
{
	buildBoardFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
	mov.resize(MAX_MOVES);
}

Board::Board(std::string fen)
{
	buildBoardFromFen(fen);
	mov.resize(MAX_MOVES);
}


Board::Board(Board* board_from, Move& move)
{
	memcpy(this->board, board_from->board, 256);
	memcpy(this->pos_of_kings, board_from->pos_of_kings, 2);
	player = !(board_from->player);
	genOneMove(move);
}

void Board::buildBoardFromFen(std::string fen, bool whether_get_all)
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
		else if (i == 'k') { pos_of_kings[BLACK] = position_now; board[position_now++] = B_JIANG; }
		else if (i == 'c') board[position_now++] = B_PAO;
		else if (i == 'p') board[position_now++] = B_BING;
		else if (i == 'R') board[position_now++] = R_JU;
		else if (i == 'N') board[position_now++] = R_MA;
		else if (i == 'B') board[position_now++] = R_XIANG;
		else if (i == 'A') board[position_now++] = R_SHI;
		else if (i == 'K') { pos_of_kings[RED] = position_now; board[position_now++] = R_JIANG; }
		else if (i == 'C') board[position_now++] = R_PAO;
		else if (i == 'P') board[position_now++] = R_BING;
	}
	if (whether_get_all)
		generateMoves();
}

void Board::genOneMove(std::string& move, bool whether_get_all)
{
	unsigned char start_position = ((12 - move[1] - '0') << 4) + move[0] - 'a' + 4;
	unsigned char end_position = ((12 - move[3] - '0') << 4) + move[2] - 'a' + 4;
	if (board[start_position] == EMPTY)
		return;
	if (board[start_position] == R_JIANG)
		pos_of_kings[RED] = end_position;
	if (board[start_position] == B_JIANG)
		pos_of_kings[BLACK] = end_position;
	board[end_position] = board[start_position];
	board[start_position] = EMPTY;
	if(whether_get_all)
		generateMoves();
}

void Board::genOneMove(Move& move, bool whether_get_all)
{
	if (board[move.from] == EMPTY)
		return;
	if (board[move.from] == R_JIANG)
		pos_of_kings[RED] = move.to;
	if (board[move.from] == B_JIANG)
		pos_of_kings[BLACK] = move.to;
	board[move.to] = board[move.from];
	board[move.from] = EMPTY;
	if (whether_get_all)
		generateMoves();
}

void Board::deleteOneMove(Move& m) 
{
	if (board[m.to] == R_JIANG)
		pos_of_kings[RED] = m.from;
	if (board[m.to] == B_JIANG)
		pos_of_kings[BLACK] = m.from;
	board[m.from] = board[m.to];
	board[m.to] = m.chessOnTo;
}

void Board::printBoardForDebug() 
{
	Log().info("当前棋盘为：");
	for (int i = 0; i < 256; i++) 
	{
		if ((i >> 4) >= 3 && (i >> 4) <= 12 && (i & 15) == 2) //打印列标
		{
			// std::cout << 12 - ((i >> 4)) << " ";
			Log().add(std::to_string(12 - ((i >> 4)))+" ", false);
			continue;
		}
		if ((i >> 4) == 14 && (i & 15) >= 4 && (i & 15) <= 12) //打印行标
		{
			// std::cout << char((i & 15) - 4 + 'a') << " ";
			Log().add(std::string("")+char((i & 15) - 4 + 'a')+" ", false);
			continue;
		}
		switch (board[i])
		{
			case BOUNDARY:
				// std::cout << "  ";
				Log().add("  ", false);
				break;
			case B_JU:
				// std::cout << "r ";
				Log().add("r ", false);
				break;
			case B_MA:
				// std::cout << "n ";
				Log().add("n ", false);
				break;
			case B_XIANG:
				// std::cout << "b ";
				Log().add("b ", false);
				break;
			case B_SHI:
				// std::cout << "a ";
				Log().add("a ", false);
				break;
			case B_JIANG:
				// std::cout << "k ";
				Log().add("k ", false);
				break;
			case B_PAO:
				// std::cout << "c ";
				Log().add("c ", false);
				break;
			case B_BING:
				// std::cout << "p ";
				Log().add("p ", false);
				break;
			case R_JU:
				// std::cout << "R ";
				Log().add("R ", false);
				break;
			case R_MA:
				// std::cout << "N ";
				Log().add("N ", false);
				break;
			case R_XIANG:
				// std::cout << "B ";
				Log().add("B ", false);
				break;
			case R_SHI:
				// std::cout << "A ";
				Log().add("A ", false);
				break;
			case R_JIANG:
				// std::cout << "K ";
				Log().add("K ", false);
				break;
			case R_PAO:
				// std::cout << "C ";
				Log().add("C ", false);
				break;
			case R_BING:
				// std::cout << "P ";
				Log().add("P ", false);
				break;
			case EMPTY:
				// std::cout << ". ";
				Log().add(". ", false);
				break;
			default:
				// std::cout << "? ";
				Log().add("? ", false);
				break;
		}
		if (i % 16 == 15) 
			Log().add("\n", false);
	}
}

void Board::printBoardForDebug2()
{
	//Log().info("当前棋盘为：");
	for (int i = 0; i < 256; i++)
	{
		if ((i >> 4) >= 3 && (i >> 4) <= 12 && (i & 15) == 2) //打印列标
		{
			std::cout << 12 - ((i >> 4)) << " ";
			//Log().add(std::to_string(12 - ((i >> 4)))+" ", false);
			continue;
		}
		if ((i >> 4) == 14 && (i & 15) >= 4 && (i & 15) <= 12) //打印行标
		{
			std::cout << char((i & 15) - 4 + 'a') << " ";
			//Log().add(std::string("")+char((i & 15) - 4 + 'a')+" ", false);
			continue;
		}
		switch (board[i])
		{
			case BOUNDARY:
				std::cout << "  ";
				//Log().add("  ", false);
				break;
			case B_JU:
				std::cout << "r ";
				//Log().add("r ", false);
				break;
			case B_MA:
				std::cout << "n ";
				//Log().add("n ", false);
				break;
			case B_XIANG:
				std::cout << "b ";
				//Log().add("b ", false);
				break;
			case B_SHI:
				std::cout << "a ";
				//Log().add("a ", false);
				break;
			case B_JIANG:
				std::cout << "k ";
				//Log().add("k ", false);
				break;
			case B_PAO:
				std::cout << "c ";
				//Log().add("c ", false);
				break;
			case B_BING:
				std::cout << "p ";
				//Log().add("p ", false);
				break;
			case R_JU:
				std::cout << "R ";
				//Log().add("R ", false);
				break;
			case R_MA:
				std::cout << "N ";
				//Log().add("N ", false);
				break;
			case R_XIANG:
				std::cout << "B ";
				//Log().add("B ", false);
				break;
			case R_SHI:
				std::cout << "A ";
				//Log().add("A ", false);
				break;
			case R_JIANG:
				std::cout << "K ";
				//Log().add("K ", false);
				break;
			case R_PAO:
				std::cout << "C ";
				//Log().add("C ", false);
				break;
			case R_BING:
				std::cout << "P ";
				//Log().add("P ", false);
				break;
			case EMPTY:
				std::cout << ". ";
				//Log().add(". ", false);
				break;
			default:
				std::cout << "? ";
				//Log().add("? ", false);
				break;
		}
		if (i % 16 == 15)
			std::cout << std::endl;
			//Log().add("\n", false);
	}
}

Move Board::randomRunMove() 
{
	Move x("a0i9");
	if (mov.empty())
		return x;
	int index = rand() % mov.size();		
	return mov[index];
}

void Board::updMovs()
{
	for (auto &m : mov)
	{
		m.chessOnTo = board[m.to];
	}
}

bool Board::checkJiang(Move& mov)
{
	genOneMove(mov, NOT_GET_ALL);
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
	for (int i = pos_of_kings[player] + 16; board[i] != BOUNDARY; i += 16)
	{
		if (board[i] == EMPTY) continue;
		else
		{
			//被兵将军
			if (i == pos_of_kings[player] + 16)
			{
				if ((player == BLACK && board[i] == R_BING) || (player == RED && board[i] == B_BING))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被車将军
			if (cnt == 0)
			{
				if ((player == BLACK && board[i] == R_JU) || (player == RED && board[i] == B_JU))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被炮将军
			if (cnt == 1)
			{
				if ((player == BLACK && board[i] == R_PAO) || (player == RED && board[i] == B_PAO))
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
	for (int i = pos_of_kings[player] - 16; board[i] != BOUNDARY; i -= 16)
	{
		if (board[i] == EMPTY) continue;
		else
		{
			//被兵将军
			if (i == pos_of_kings[player] - 16)
			{
				if ((player == BLACK && board[i] == R_BING) || (player == RED && board[i] == B_BING))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被車将军
			if (cnt == 0)
			{
				if ((player == BLACK && board[i] == R_JU) || (player == RED && board[i] == B_JU))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被炮将军
			if (cnt == 1)
			{
				if ((player == BLACK && board[i] == R_PAO) || (player == RED && board[i] == B_PAO))
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
	for (int i = pos_of_kings[player] - 1; board[i] != BOUNDARY; i--)
	{
		if (board[i] == EMPTY) continue;
		else
		{
			//被兵将军
			if (i == pos_of_kings[player] - 1)
			{
				if ((player == BLACK && board[i] == R_BING) || (player == RED && board[i] == B_BING))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被車将军
			if (cnt == 0)
			{
				if ((player == BLACK && board[i] == R_JU) || (player == RED && board[i] == B_JU))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被炮将军
			if (cnt == 1)
			{
				if ((player == BLACK && board[i] == R_PAO) || (player == RED && board[i] == B_PAO))
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
	for (int i = pos_of_kings[player] + 1; board[i] != BOUNDARY; i++)
	{
		if (board[i] == EMPTY) continue;
		else
		{
			//被兵将军
			if (i == pos_of_kings[player] + 1)
			{
				if ((player == BLACK && board[i] == R_BING) || (player == RED && board[i] == B_BING))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被車将军
			if (cnt == 0)
			{
				if ((player == BLACK && board[i] == R_JU) || (player == RED && board[i] == B_JU))
				{
					deleteOneMove(mov);
					return true;
				}
			}
			//被炮将军
			if (cnt == 1)
			{
				if ((player == BLACK && board[i] == R_PAO) || (player == RED && board[i] == B_PAO))
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
		int to = pos_of_kings[player] + MA_Feasible[i];
		int leg = to - MA_Leg[i];
		if (board[leg] == EMPTY)
		{
			if ((player == BLACK && board[to] == R_MA) || (player == RED && board[to] == B_MA))
			{
				deleteOneMove(mov);
				return true;
			}
		}
	}
	deleteOneMove(mov);
	return false;
}

bool Board::checkJiang(Move& mov, bool side)
{
	genOneMove(mov, NOT_GET_ALL);
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
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
					}
					else if (board[temp] >= 100 && board[temp] < 255)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
					}
					else if (board[temp] <= 100)
					{
						Move Curstep(index, temp);
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						Curstep.chessOnTo = board[index + MA_Feasible[i]];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						Curstep.chessOnTo = board[index + MA_Feasible[i]];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						Curstep.chessOnTo = board[index + XIANG_Feasible[i]];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						Curstep.chessOnTo = board[index + XIANG_Feasible[i]];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						Curstep.chessOnTo = board[index + SHI_Feasible[i]];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						Curstep.chessOnTo = board[index + SHI_Feasible[i]];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						Curstep.chessOnTo = board[index + JIANG_Feasible[i]];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						Curstep.chessOnTo = board[index + JIANG_Feasible[i]];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						Curstep.chessOnTo = board[temp];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
							Curstep.chessOnTo = board[index + BING_R_Feasible[i]];
							if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
						}
					}
				}
				else
				{
					if (board[index - 0x10] != 255 && (!board[index - 0x10] || board[index - 0x10] >= 100))
					{
						Move Curstep(index, index - 0x10);	//需存储
						Curstep.chessOnTo = board[index - 0x10];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
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
							Curstep.chessOnTo = board[index + BING_B_Feasible[i]];
							if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
						}
					}
				}
				else
				{
					if (board[index + 0x10] != 255 && board[index + 0x10] <= 100)
					{
						Move Curstep(index, index + 0x10);	//需存储
						Curstep.chessOnTo = board[index + 0x10];
						if (!checkJiang(Curstep)) mov.emplace_back(Curstep);
					}
				}
				break;
			}
		}
	}
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

GameStatus Board::mtcsMove()
{
	uint32_t round = 0;
	bool cur_side = player;
	Move temp_mov;
	// 随机数
	unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	// 模拟棋局
	while (round < 60)
	{
		// 生成走法
		generateMoves(cur_side);
		// 胜负
		if (mov.size() == 0)
		{
			if (cur_side == player) 
				return LOSS;
			else 
				return WIN;
		}
		/*
		* 调节选择吃子概率
		while (true)
		{
			temp_mov = mov[generator() % mov.size()];
			if (board[temp_mov.to] == EMPTY)
			{
				if (generator() % 100 > 60) break;
			}
			else break;
		}
		*/
		temp_mov = mov[generator() % mov.size()];
		// 吃子判断
		if (board[temp_mov.to] != EMPTY)
			round = 0;
		else round++;

		// std::cout << mov.size() << " " << round << std::endl;

		// printBoardForDebug2();
		// getchar();

		// 走子
		genOneMove(temp_mov);
		// getchar();
		cur_side = !cur_side;
	}
	return TIE;
}
