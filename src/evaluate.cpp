/*****************************************************************/ /**
 * \file   evaluate.cpp
 * \brief  估值函数源文件
 * 
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
#include <iostream>

#include "evaluate.h"
#include "base.h"
#include "log.h"
#include "board.h"

/**
 * @brief 将黑棋的位置映射为红棋
 * 
 * @param pos 黑棋在数组中的位置
 * @return int 
 */
int transferPosOfBlack(int pos)
{
	return 256 - pos;
}

// 棋子基础子力分
int baseValue(unsigned char* board, player_type player, int state)
{
	int ans = 0;
	for (int i = 0x34; i <= 0xcc; i++)
	{
		if (player == RED_PLAYER && board[i] > 0 && board[i] < 8)
		{
			ans += evaluate::valChessBase[state][board[i]];
		}
		if (player == BLACK_PLAYER && board[i] > 100 && board[i] < 108)
		{
			ans += evaluate::valChessBase[state][board[i] - 100];
		}
	}
	return ans;
}

// 棋子位置分
int posValue(unsigned char* board, player_type player)
{
	int ans = 0;
	for (int i = 0x34; i <= 0xcc; i++)
	{
		if (player == RED_PLAYER && board[i] > 0 && board[i] < 8)
		{
			ans += evaluate::valChessPos[board[i]][i];
		}
		if (player == BLACK_PLAYER && board[i] > 100 && board[i] < 108)
		{
			ans += evaluate::valChessPos[board[i] - 100][transferPosOfBlack(i)];
		}
	}
	return ans;
}

// 棋子灵活度评分
int moveValue(unsigned char* board, std::vector<Move>& move_vec, player_type player)
{
	int ans = 0;
	for (auto m : move_vec)
	{
		unsigned char chess = board[m.from];
		// std::cout << (int)m.from << " " << (int)chess << std::endl;
		if (player == RED_PLAYER && chess > 0 && chess < 8)
		{
			ans += evaluate::valChessMove[chess];
		}
		if (player == BLACK_PLAYER && chess > 100 && chess < 108)
		{
			ans += evaluate::valChessMove[chess - 100];
		}
	}
	return ans;
}

// 更新局面评估函数
int evaluate::updBoardValue(Board& gameBoard)
{
	// 查看对局进度
	int num = 0, state = 0;
	for (int i = 0x34; i <= 0xcc; i++)
	{
		if (gameBoard.board[i] != EMPTY && gameBoard.board[i] != BOUNDARY)
			num++;
	}
	if (num >= 28)
		state = 0;
	else if (num >= 14)
		state = 1;
	else
		state = 2;
	// 
	gameBoard.generateMoves(RED_PLAYER);
	gameBoard.redValue = baseValue(gameBoard.board, RED_PLAYER, state) + posValue(gameBoard.board, RED_PLAYER) + moveValue(gameBoard.board, gameBoard.move_vec, RED_PLAYER);
	gameBoard.generateMoves(BLACK_PLAYER);
	gameBoard.blackValue = baseValue(gameBoard.board, BLACK_PLAYER, state) + posValue(gameBoard.board, BLACK_PLAYER) + moveValue(gameBoard.board, gameBoard.move_vec, BLACK_PLAYER);
	
	if (gameBoard.player == RED_PLAYER) {
		gameBoard.gameVal = gameBoard.redValue - gameBoard.blackValue;
	}
	else {
		gameBoard.gameVal = gameBoard.blackValue - gameBoard.redValue;
	}
	
	return gameBoard.gameVal;
}



