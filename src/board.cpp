#include <iostream>
#include <random>
#include <chrono>

#include "board.h"
#include "base.h"
#include "log.h"
#include "evaluate.h"

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
	evaluate::updBoardValue(*this);
}

void Board::genOneMove(std::string& move)
{
	Move tmp_move(move);
	evaluate::updMovValue(*this, tmp_move);

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
}

void Board::genOneMove(Move& move)
{
	evaluate::updMovValue(*this, move);

	if (board[move.from] == EMPTY)
		return;
	if (board[move.from] == R_JIANG)
		pos_of_kings[RED] = move.to;
	if (board[move.from] == B_JIANG)
		pos_of_kings[BLACK] = move.to;
	board[move.to] = board[move.from];
	board[move.from] = EMPTY;
}

void Board::deleteOneMove(Move& m) 
{
	evaluate::deleteMovValue(*this, m);

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


GameStatus Board::mctsMove()
{
	uint32_t round = 0, all_round = 0;
	bool cur_side = player;
	Move temp_mov;
	// 随机数
	unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	// 模拟棋局
	while (round < 60)
	{
		all_round++;
		// 生成走法
		generateMoves(cur_side);
		// 胜负
		if (mov.size() == 0)
		{
			if (cur_side == player) 
				return LOSS;
			else 
				return all_round;
		}
		temp_mov = mov[generator() % mov.size()];
		// 吃子判断
		if (board[temp_mov.to] != EMPTY)
			round = 0;
		else round++;

		// 走子
		genOneMove(temp_mov);
		cur_side = !cur_side;
	}
	return TIE;
}

void Board::updGameVal()
{
	evaluate::updBoardValue(*this);
	if (player == RED)
		gameVal = redValue - blackValue;
	else
		gameVal = blackValue - redValue;
}
