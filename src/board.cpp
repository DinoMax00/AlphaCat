#include <iostream>
#include <random>

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
	if (whether_get_all)
		generateMoves();
}

void Board::genOneMove(std::string& move, bool whether_get_all)
{
	unsigned char start_position = ((12 - move[1] - '0') << 4) + move[0] - 'a' + 4;
	unsigned char end_position = ((12 - move[3] - '0') << 4) + move[2] - 'a' + 4;
	if (board[start_position] == EMPTY)
		return;
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
	board[m.from] = board[m.to];
	board[m.to] = board[m.chessOnTo];
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

Move Board::randomRunMove() 
{
	Move x("a0i9");
	if (mov.empty())
		return x;
	int t = 1000;
	while(t --)
	{
		int index = rand() % mov.size();
		Log().info(mov[index].moveToString());
		if (checkJiang(mov[index]))
			continue;
		printBoardForDebug();
		return mov[index];
	}
	return x;
}

void Board::updMovs()
{
	for (auto m : mov)
	{
		m.chessOnTo = board[m.to];
	}
}





bool Board::checkJiang(Move& mov)
{
	genOneMove(mov, 0);
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
				if ((player == BLACK && board[i] == R_JU) || (player == RED && board[i] == B_JU))
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
				if ((player == BLACK && board[i] == R_JU) || (player == RED && board[i] == B_JU))
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
				if ((player == BLACK && board[i] == R_JU) || (player == RED && board[i] == B_JU))
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
				if ((player == BLACK && board[i] == R_JU) || (player == RED && board[i] == B_JU))
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
		int leg = to - MA_Leg[player];
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
