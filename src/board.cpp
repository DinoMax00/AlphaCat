#include"board.h"
#include"base.h"
#include<iostream>
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
