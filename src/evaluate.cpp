#include <iostream>

#include "evaluate.h"
#include "base.h"
#include "log.h"
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
			gameBoard.blackValue += evaluate::valChessPos[gameBoard.board[i] - 100][evaluate::transferPosOfBlack(i)];
	}
}

void evaluate::updMovValue(Board& gameBoard, Move& mov)
{
	// std::cout << "这样移动:" << (int)mov.from << " " << (int)gameBoard.board[mov.from]  << "---"<< (int)mov.to << " " << (int)gameBoard.board[mov.to] << std::endl;
	// std::cout << "原来是:" << (int)gameBoard.redValue << " " << (int)gameBoard.blackValue << std::endl;
	if (gameBoard.board[mov.from] < 8)
	{
		//  吃子
		if (gameBoard.board[mov.to] > 100 && gameBoard.board[mov.to] < 108)
			gameBoard.blackValue -= evaluate::valChessPos[gameBoard.board[mov.to] - 100][evaluate::transferPosOfBlack(mov.to)];
		// 更新
		/*
		Log().info("红色");
		Log().add(evaluate::valChessPos[gameBoard.board[mov.from]][mov.from]);
		Log().add(evaluate::valChessPos[gameBoard.board[mov.from]][mov.to]);
		Log().add("红色------");
		*/
		
		gameBoard.redValue -= evaluate::valChessPos[gameBoard.board[mov.from]][mov.from];
		gameBoard.redValue += evaluate::valChessPos[gameBoard.board[mov.from]][mov.to];

	}
	else
	{
		//  吃子
		if (gameBoard.board[mov.to] > 0 && gameBoard.board[mov.to] < 8)
			gameBoard.redValue -= evaluate::valChessPos[gameBoard.board[mov.to]][mov.to];
		// 更新
		gameBoard.blackValue -= evaluate::valChessPos[gameBoard.board[mov.from] - 100][evaluate::transferPosOfBlack(mov.from)];
		gameBoard.blackValue += evaluate::valChessPos[gameBoard.board[mov.from] - 100][evaluate::transferPosOfBlack(mov.to)];
		/*
		Log().info("黑色");
		Log().add(evaluate::valChessPos[gameBoard.board[mov.from] - 100][evaluate::transferPosOfBlack(mov.from)]);
		Log().add(evaluate::valChessPos[gameBoard.board[mov.from] - 100][evaluate::transferPosOfBlack(mov.to)]);
		Log().add("黑色------");
		*/
		
	}
	// std::cout << "现在是:" << gameBoard.redValue << " " << gameBoard.blackValue << std::endl;
}

void evaluate::deleteMovValue(Board& gameBoard, Move& mov)
{
	// std::cout << "--这样移动:" << (int)mov.from << " " << (int)gameBoard.board[mov.from] << "---" << (int)mov.to << " " << (int)gameBoard.board[mov.to] << std::endl;
	// std::cout << "原来是:" << (int)gameBoard.redValue << " " << (int)gameBoard.blackValue << std::endl;
	if (gameBoard.board[mov.from] < 8)
	{
		//  吐子
		if (mov.chessOnTo > 100 && mov.chessOnTo < 108)
			gameBoard.blackValue += evaluate::valChessPos[mov.chessOnTo - 100][evaluate::transferPosOfBlack(mov.to)];
		// 更新
		gameBoard.redValue -= evaluate::valChessPos[gameBoard.board[mov.to]][mov.to];
		gameBoard.redValue += evaluate::valChessPos[gameBoard.board[mov.to]][mov.from];

	}
	else
	{
		//  吐子
		if (mov.chessOnTo > 0 && mov.chessOnTo < 8)
			gameBoard.redValue += evaluate::valChessPos[mov.chessOnTo][mov.to];
		// 更新
		gameBoard.blackValue -= evaluate::valChessPos[gameBoard.board[mov.to] - 100][evaluate::transferPosOfBlack(mov.to)];
		gameBoard.blackValue += evaluate::valChessPos[gameBoard.board[mov.to] - 100][evaluate::transferPosOfBlack(mov.from)];

	}
	// std::cout << "现在是:" << gameBoard.redValue << " " << gameBoard.blackValue << std::endl;
}