#include <iostream>

#include "evaluate.h"
#include "base.h"
#include "board.h"


int evaluate::transferPosOfBlack(int pos)
{
	return 256 - pos;
}


void evaluate::updBoardValue(Board& gameBoard)
{
	gameBoard.redValue = gameBoard.blackValue = 0;
	for (int i = 0; i < 256; i++)
	{
		if (gameBoard.board[i] == EMPTY || gameBoard.board[i] == BOUNDARY)
			continue;
		if (gameBoard.board[i] < 8) // 红棋
			gameBoard.redValue += evaluate::valChessPos[gameBoard.board[i]][i];
		else // 黑棋
			gameBoard.redValue += evaluate::valChessPos[gameBoard.board[i]-100][evaluate::transferPosOfBlack(i)];
	}
}

void evaluate::updMovValue(Board& gameBoard, Move& mov)
{
	if (gameBoard.board[mov.from] < 8)
	{
		//  吃子
		if(gameBoard.board[mov.to] != EMPTY)
			gameBoard.blackValue -= evaluate::valChessPos[gameBoard.board[mov.to] - 100][evaluate::transferPosOfBlack(mov.to)];
		// 更新
		gameBoard.redValue -= evaluate::valChessPos[gameBoard.board[mov.from]][mov.from];
		gameBoard.redValue += evaluate::valChessPos[gameBoard.board[mov.to]][mov.to];
	}
	else
	{
		//  吃子
		if (gameBoard.board[mov.to] != EMPTY)
			gameBoard.redValue -= evaluate::valChessPos[gameBoard.board[mov.to]][mov.to];
		// 更新
		gameBoard.blackValue -= evaluate::valChessPos[gameBoard.board[mov.from]-100][evaluate::transferPosOfBlack(mov.from)];
		gameBoard.blackValue += evaluate::valChessPos[gameBoard.board[mov.to]-100][evaluate::transferPosOfBlack(mov.to)];
	}
}