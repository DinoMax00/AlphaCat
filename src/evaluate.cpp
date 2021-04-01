/*****************************************************************/ /**
 * \file   evaluate.cpp
 * \brief  ???????????
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

void evaluate::updBoardValue(Board &gameBoard)
{
	gameBoard.redValue = gameBoard.blackValue = 0;
	for (int i = 0; i < 256; i++)
	{
		if (gameBoard.board[i] < 8 && gameBoard.board[i] > 0)
			gameBoard.redValue += evaluate::valChessPos[gameBoard.board[i]][i];
		else if (gameBoard.board[i] < 108 && gameBoard.board[i] > 100)
			gameBoard.blackValue += evaluate::valChessPos[gameBoard.board[i] - 100][transferPosOfBlack(i)];
	}
	
	if (gameBoard.player == RED_PLAYER)
		gameBoard.gameVal = gameBoard.redValue - gameBoard.blackValue;
	else
		gameBoard.gameVal = gameBoard.blackValue - gameBoard.redValue;
}

void evaluate::updMovValue(Board &gameBoard, Move &mov)
{
	if (gameBoard.board[mov.from] < 8)
	{
		if (gameBoard.board[mov.to] > 100 && gameBoard.board[mov.to] < 108)
			gameBoard.blackValue -= evaluate::valChessPos[gameBoard.board[mov.to] - 100][transferPosOfBlack(mov.to)];
		gameBoard.redValue -= evaluate::valChessPos[gameBoard.board[mov.from]][mov.from];
		gameBoard.redValue += evaluate::valChessPos[gameBoard.board[mov.from]][mov.to];
	}
	else
	{
		if (gameBoard.board[mov.to] > 0 && gameBoard.board[mov.to] < 8)
			gameBoard.redValue -= evaluate::valChessPos[gameBoard.board[mov.to]][mov.to];

		gameBoard.blackValue -= evaluate::valChessPos[gameBoard.board[mov.from] - 100][transferPosOfBlack(mov.from)];
		gameBoard.blackValue += evaluate::valChessPos[gameBoard.board[mov.from] - 100][transferPosOfBlack(mov.to)];
	}
	if (gameBoard.player == RED_PLAYER)
		gameBoard.gameVal = gameBoard.redValue - gameBoard.blackValue;
	else
		gameBoard.gameVal = gameBoard.blackValue - gameBoard.redValue;
}

void evaluate::deleteMovValue(Board &gameBoard, Move &mov)
{
	if (gameBoard.board[mov.to] < 8)
	{
		if (mov.chessOnTo > 100 && mov.chessOnTo < 108)
			gameBoard.blackValue += evaluate::valChessPos[mov.chessOnTo - 100][transferPosOfBlack(mov.to)];

		gameBoard.redValue -= evaluate::valChessPos[gameBoard.board[mov.to]][mov.to];
		gameBoard.redValue += evaluate::valChessPos[gameBoard.board[mov.to]][mov.from];
	}
	else
	{
		if (mov.chessOnTo > 0 && mov.chessOnTo < 8)
			gameBoard.redValue += evaluate::valChessPos[mov.chessOnTo][mov.to];

		gameBoard.blackValue -= evaluate::valChessPos[gameBoard.board[mov.to] - 100][transferPosOfBlack(mov.to)];
		gameBoard.blackValue += evaluate::valChessPos[gameBoard.board[mov.to] - 100][transferPosOfBlack(mov.from)];
	}
	if (gameBoard.player == RED_PLAYER)
		gameBoard.gameVal = gameBoard.redValue - gameBoard.blackValue;
	else
		gameBoard.gameVal = gameBoard.blackValue - gameBoard.redValue;
}