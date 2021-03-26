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
void Board::printBoardForDebug() 
{
	for (int i = 0; i < 256; i++) 
	{
		switch (board[i])
		{
			case 255:
				std::cout << "  ";
				break;
			case 101:
				std::cout << "r ";
				break;
			case 102:
				std::cout << "n ";
				break;
			case 103:
				std::cout << "b ";
				break;
			case 104:
				std::cout << "a ";
				break;
			case 105:
				std::cout << "k ";
				break;
			case 106:
				std::cout << "c ";
				break;
			case 107:
				std::cout << "p ";
				break;
			case 1:
				std::cout << "R ";
				break;
			case 2:
				std::cout << "N ";
				break;
			case 3:
				std::cout << "B ";
				break;
			case 4:
				std::cout << "A ";
				break;
			case 5:
				std::cout << "K ";
				break;
			case 6:
				std::cout << "C ";
				break;
			case 7:
				std::cout << "P ";
				break;
			case 0:
				std::cout << "· ";
				break;
			default:
				std::cout << "? ";
				break;
		}
		if (i % 16 == 15) std::cout << std::endl;
	}
}
