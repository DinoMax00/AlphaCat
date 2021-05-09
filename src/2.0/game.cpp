#include <string.h>
#include <iostream>
#include "game.h"


/* 匈牙利记号约定：
 *
 * sq: 格子序号(整数，从0到255，参阅"pregen.cpp")
 * pc: 棋子序号(整数，从0到47，参阅"position.cpp")
 * pt: 棋子类型序号(整数，从0到6，参阅"position.cpp")
 * mv: 着法(整数，从0到65535，参阅"position.cpp")
 * sd: 走子方(整数，0代表红方，1代表黑方)
 * vl: 局面价值(整数，从"-MATE_VALUE"到"MATE_VALUE"，参阅"position.cpp")
 * (注：以上五个记号可与uc、dw等代表整数的记号配合使用)
 * pos: 局面(PositionStruct类型，参阅"position.h")
 * sms: 位行和位列的着法生成预置结构(参阅"pregen.h")
 * smv: 位行和位列的着法判断预置结构(参阅"pregen.h")
 */

Game::Game()
{
	// 清空棋盘
	std::memset(this->board, 0, sizeof(this->board));
	std::memset(this->pieces, 0, sizeof(this->pieces));
	// 初始化游戏角色
	this->cur_player = RED;
	this->red_val = this->black_val = 0;
}

void Game::initGame()
{
	// 清空棋盘
	std::memset(this->board, 0, sizeof(this->board));
	std::memset(this->pieces, 0, sizeof(this->pieces));
	// 初始化游戏角色
	this->cur_player = RED;
	this->red_val = this->black_val = 0;
}

void Game::putChess(int32_t sq, int32_t pc, bool del)
{
	if (del)
	{
		this->board[sq] = 0;
		this->pieces[sq] = 0;
	}
	else
	{
		this->board[sq] = pc;
		this->pieces[pc] = sq;
	}
	// 更新估值

	// 更新zobr键值
}

void Game::changePlayer()
{
	this->cur_player = !this->cur_player;
}

void Game::buildFromFen(std::string fen)
{
	this->initGame();
	// 两个数组存储各类棋子对应的标号
	uint8_t red_idx[7] = {JIANG_FROM, SHI_FROM, XIANG_FROM, 
							MA_FROM, JU_FROM, PAO_FROM, BING_FROM};
	uint8_t black_idx[7] = { JIANG_FROM, SHI_FROM, XIANG_FROM,
							MA_FROM, JU_FROM, PAO_FROM, BING_FROM };
	for (int i = 0; i < 7; i++)
	{
		red_idx[i] += 16;
		black_idx[i] += 32;
	}

	uint8_t pos = 0;
	uint8_t i = BOARD_TOP, j = BOARD_LEFT;
	uint8_t len = (uint8_t)fen.length();
	while (pos < len)
	{
		char ch = fen[pos];
		if (ch == '/')
		{
			// 换行
			j = BOARD_LEFT;
			i++;
			if (i > BOARD_BOTTOM)
				break;
		}
		else if (isdigit(ch))
		{
			// 空格
			j += (uint8_t)(ch - '0');
		}
		else if (ch >= 'A' && ch <= 'Z' && j <= BOARD_RIGHT)
		{
			// 红棋
			uint8_t temp = getChessNumber(ch);
			putChess(coordToPos(i, j), red_idx[temp]);
			red_idx[temp]++;
			j++;
		}
		else if (ch >= 'a' && ch <= 'z' && j <= BOARD_RIGHT)
		{
			// 黑棋
			uint8_t temp = getChessNumber(ch + 'A' - 'a');
			putChess(coordToPos(i, j), black_idx[temp]);
			black_idx[temp]++;
			j++;
		}
		pos++;
	}
}

void Game::takeOneMove(int32_t move)
{
	int src = getSrc(move);
	int dst = getDst(move);
	int chessOnSrc = this->board[src];
	int chessOnDst = this->board[dst];
	
	if (chessOnDst)
	{
		// 目标位置有子
		this->pieces[chessOnDst] = 0;
	}
	else
	{
		//
	}

	this->board[src] = 0;
	this->board[dst] = chessOnSrc;
	this->pieces[chessOnSrc] = dst;
}

void Game::printForDebug()
{
	std::cout << "______________________\n";
	// 打印棋盘
	int row = 9;
	for (int i = 0x33; i <= 0xcb; i += 16)
	{
		std::cout << row-- << " |";
		for (int j = i; j <= i + 8; j++)
		{
			int temp = this->board[j];
			if (temp == 0) std::cout << "*";
			else if (temp >= 32) {
				temp -= 32;
				if (temp >= 11) {
					std::cout << "b";
				}
				else if (temp >= 9) {
					std::cout << "p";
				}
				else if (temp >= 7) {
					std::cout << "c";
				}
				else if (temp >= 5) {
					std::cout << "m";
				}
				else if (temp >= 3) {
					std::cout << "x";
				}
				else if (temp >= 1) {
					std::cout << "s";
				}
				else if (temp >= 0) {
					std::cout << "j";
				}
			}
			else if (temp >= 16) {
				temp -= 16;
				if (temp >= 11) {
					std::cout << "B";
				}
				else if (temp >= 9) {
					std::cout << "P";
				}
				else if (temp >= 7) {
					std::cout << "C";
				}
				else if (temp >= 5) {
					std::cout << "M";
				}
				else if (temp >= 3) {
					std::cout << "X";
				}
				else if (temp >= 1) {
					std::cout << "S";
				}
				else if (temp >= 0) {
					std::cout << "J";
				}
			}
			std::cout << " ";
		}
		std::cout << "|\n";
	}
	// 
	std::cout << "----------------------\n";
	std::cout << "   a b c d e f g h i\n";
}
