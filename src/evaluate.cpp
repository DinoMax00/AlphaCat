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
		if (gameBoard.board[i] < 8) // ����
			gameBoard.redValue += evaluate::valChessPos[gameBoard.board[i]][i];
		else // ����
			gameBoard.blackValue += evaluate::valChessPos[gameBoard.board[i] - 100][evaluate::transferPosOfBlack(i)];
	}
}

void evaluate::updMovValue(Board& gameBoard, Move& mov)
{
	// std::cout << "�����ƶ�:" << (int)mov.from << " " << (int)gameBoard.board[mov.from]  << "---"<< (int)mov.to << " " << (int)gameBoard.board[mov.to] << std::endl;
	// std::cout << "ԭ����:" << (int)gameBoard.redValue << " " << (int)gameBoard.blackValue << std::endl;
	if (gameBoard.board[mov.from] < 8)
	{
		//  ����
		if (gameBoard.board[mov.to] > 100 && gameBoard.board[mov.to] < 108)
			gameBoard.blackValue -= evaluate::valChessPos[gameBoard.board[mov.to] - 100][evaluate::transferPosOfBlack(mov.to)];
		// ����
		/*
		Log().info("��ɫ");
		Log().add(evaluate::valChessPos[gameBoard.board[mov.from]][mov.from]);
		Log().add(evaluate::valChessPos[gameBoard.board[mov.from]][mov.to]);
		Log().add("��ɫ------");
		*/
		
		gameBoard.redValue -= evaluate::valChessPos[gameBoard.board[mov.from]][mov.from];
		gameBoard.redValue += evaluate::valChessPos[gameBoard.board[mov.from]][mov.to];

	}
	else
	{
		//  ����
		if (gameBoard.board[mov.to] > 0 && gameBoard.board[mov.to] < 8)
			gameBoard.redValue -= evaluate::valChessPos[gameBoard.board[mov.to]][mov.to];
		// ����
		gameBoard.blackValue -= evaluate::valChessPos[gameBoard.board[mov.from] - 100][evaluate::transferPosOfBlack(mov.from)];
		gameBoard.blackValue += evaluate::valChessPos[gameBoard.board[mov.from] - 100][evaluate::transferPosOfBlack(mov.to)];
		/*
		Log().info("��ɫ");
		Log().add(evaluate::valChessPos[gameBoard.board[mov.from] - 100][evaluate::transferPosOfBlack(mov.from)]);
		Log().add(evaluate::valChessPos[gameBoard.board[mov.from] - 100][evaluate::transferPosOfBlack(mov.to)]);
		Log().add("��ɫ------");
		*/
		
	}
	// std::cout << "������:" << gameBoard.redValue << " " << gameBoard.blackValue << std::endl;
}

void evaluate::deleteMovValue(Board& gameBoard, Move& mov)
{
	// std::cout << "--�����ƶ�:" << (int)mov.from << " " << (int)gameBoard.board[mov.from] << "---" << (int)mov.to << " " << (int)gameBoard.board[mov.to] << std::endl;
	// std::cout << "ԭ����:" << (int)gameBoard.redValue << " " << (int)gameBoard.blackValue << std::endl;
	if (gameBoard.board[mov.from] < 8)
	{
		//  ����
		if (mov.chessOnTo > 100 && mov.chessOnTo < 108)
			gameBoard.blackValue += evaluate::valChessPos[mov.chessOnTo - 100][evaluate::transferPosOfBlack(mov.to)];
		// ����
		gameBoard.redValue -= evaluate::valChessPos[gameBoard.board[mov.to]][mov.to];
		gameBoard.redValue += evaluate::valChessPos[gameBoard.board[mov.to]][mov.from];

	}
	else
	{
		//  ����
		if (mov.chessOnTo > 0 && mov.chessOnTo < 8)
			gameBoard.redValue += evaluate::valChessPos[mov.chessOnTo][mov.to];
		// ����
		gameBoard.blackValue -= evaluate::valChessPos[gameBoard.board[mov.to] - 100][evaluate::transferPosOfBlack(mov.to)];
		gameBoard.blackValue += evaluate::valChessPos[gameBoard.board[mov.to] - 100][evaluate::transferPosOfBlack(mov.from)];

	}
	// std::cout << "������:" << gameBoard.redValue << " " << gameBoard.blackValue << std::endl;
}