/*****************************************************************//**
 * \file   ucci.cpp
 * \brief  ucciЭ��Դ�ļ�
 *
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
#include <iostream>
#include <string>
#include <Windows.h>

#include "search.h"
#include "ucci.h"
#include "log.h"
#include "game.h"
#include "move.h"

void UcciEngine::clear()
{
	commandStr.clear();
	commandVec.clear();
}


void UcciEngine::split()
{
	commandStr += ' ';
	std::string tmp;
	for (auto ch : commandStr)
	{
		if (ch != ' ')
		{
			tmp += ch;
		}
		else if (tmp != "")
		{
			commandVec.push_back(tmp);
			tmp.clear();
		}
	}
}


void UcciEngine::getCommand()
{
	std::getline(std::cin, commandStr);
	if (commandStr.size()) split();
	Log().info("�������->" + commandStr);

}


void UcciEngine::bootEngine()
{
	while (true)
	{
		getCommand();
		if (!commandVec.size())
			continue;
		if (commandVec[0] == "ucci")
		{
			// ���������Ϣ
			std::cout << "id name AlphaCat" << std::endl;
			std::cout << "id copyright 2021-2022 TongJi" << std::endl;
			std::cout << "id copyright 2021-2022 TongJi" << std::endl;
			std::cout << "id author Dino FeiFei PPZ AYi" << std::endl;
			std::cout << "ucciok" << std::endl;
			clear();
			return;
		}
		clear();
	}
}


void UcciEngine::updWhichPlayer()
{
	if (commandVec[1] == "fen")
	{
		// ������Ϸ��ɫ
		if (commandVec[3] == "b")
		{
			if (commandVec.size() <= 9)
				game.cur_player = BLACK;
			else if ((commandVec.size() - 9) % 2)
				game.cur_player = RED;
			else
				game.cur_player = BLACK;
		}
		else
		{
			if (commandVec.size() <= 9)
				game.cur_player = BLACK;
			else if ((commandVec.size() - 9) % 2)
				game.cur_player = RED;
			else
				game.cur_player = BLACK;
		}
	}
	else if (commandVec[1] == "startpos")
	{
		// ������Ϸ��ɫ
		if (commandVec.size() <= 3)
			game.cur_player = RED;
		else if ((commandVec.size() - 3) % 2)
			game.cur_player = BLACK;
		else
			game.cur_player = RED;
	}
}


void UcciEngine::run()
{
	while (true)
	{
		getCommand();
		if (!commandVec.size())
			continue;
		// ��������ǲ��ǻ�����
		if (commandVec[0] == "isready")
		{
			std::cout << "readyok" << std::endl;
			Log().info("������Ӧ->readyok");
		}
		// �µĶԾ�
		else if (commandVec[0] == "setoption")
		{
			if (commandVec[1] == "newgame")
			{
				game.buildFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
			}
		}
		// ����
		else if (commandVec[0] == "position")
		{
			game.buildFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
			updWhichPlayer();
			if (commandVec[1] == "fen")
			{
				// ����������Ӿ��� 
				game.buildFromFen(commandVec[2]);
				// ���º�������
				if (commandVec.size() > 9)
				{
					for (int i = 9; i < (int)commandVec.size(); i++) {
						game.takeOneMove(stringToMove(commandVec[i]));
					}
				}
			}
			else if (commandVec[1] == "startpos" && commandVec.size() > 3)
			{
				// ���¾���
				game.buildFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
				// ���º�������
				if (commandVec.size() > 3)
				{
					for (int i = 3; i < (int)commandVec.size(); i++) {
						game.takeOneMove(stringToMove(commandVec[i]));
					}
				}
			}
		}
		// ����
		else if (commandVec[0] == "go" && commandVec.size() > 2)
		{
			// game.printForDebug();
			/*
			move_sort.getAllMoves(game);
			bool flag;
			int mv;
			do {
				mv = move_sort.random();
				flag = game.takeOneMove(mv);
			} while (!flag);
			*/
			Search.pos = this->game;
			Search.pos.evaBoard();
			// Search.pos.buildFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
			// std::cout << GetTickCount() << std::endl;
			SearchMain(4);
			// std::cout << GetTickCount() << std::endl;
			// this->game.printForDebug();
			// std::cout << Search.result << std::endl;
			std::cout << "bestmove " << moveToString(Search.result) << std::endl;
			Log().info("������Ӧ->bestmove " + moveToString(Search.result));
		}
		// �ݰ�
		else if (commandVec[0] == "quit")
		{
			std::cout << "bye" << std::endl;
			Log().info("������Ӧ->bye");
			return;
		}
		clear();
	}
}