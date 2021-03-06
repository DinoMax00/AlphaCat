/*****************************************************************//**
 * \file   ucci.cpp
 * \brief  ucci协议源文件
 *
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
#include <iostream>
#include <string>
#include <Windows.h>

#include "search.h"
#include "ucci.h"
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
			// 输出引擎信息
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
		// 更新游戏角色
		if (commandVec[3] == "b")
		{
			game.cur_player = BLACK;
		}
		else
		{
			game.cur_player = RED;
		}
	}
	else if (commandVec[1] == "startpos")
	{
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
		// 检测引擎状态
		if (commandVec[0] == "isready")
		{
			std::cout << "readyok" << std::endl;
		}
		// 新的对局
		else if (commandVec[0] == "setoption")
		{
			if (commandVec[1] == "newgame")
			{
				game.buildFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
			}
		}
		// 移子
		else if (commandVec[0] == "position")
		{
			game.buildFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
			// 更新起始局面游戏角色
			updWhichPlayer();
			if (commandVec[1] == "fen")
			{
				// 更新最近吃子局面 
				game.buildFromFen(commandVec[2]);
				// 更新后续局面
				if (commandVec.size() > 9)
				{
					for (int i = 9; i < (int)commandVec.size(); i++) {
						game.takeOneMove(stringToMove(commandVec[i]));
					}
				}
			}
			else if (commandVec[1] == "startpos" && commandVec.size() > 3)
			{
				// 更新局面
				game.buildFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
				// 更新后续局面
				if (commandVec.size() > 3)
				{
					for (int i = 3; i < (int)commandVec.size(); i++) {
						game.takeOneMove(stringToMove(commandVec[i]));
					}
				}
			}
		}
		// 走子
		else if (commandVec[0] == "go" && commandVec.size() > 2)
		{
			Search.pos = this->game;
			Search.pos.evaBoard();
			Search.pos.printForDebug();
			//std::cout << GetTickCount64() << std::endl;
			searchMain(30);
			//std::cout << GetTickCount64() << std::endl;
			if (Search.result == -1) {
				std::cout << "nobestmove " << std::endl;
			}
			else {
				std::cout << "bestmove " << moveToString(Search.result) << std::endl;
			}
		}
		// 退出
		else if (commandVec[0] == "quit")
		{
			std::cout << "bye" << std::endl;
			return;
		}

		clear();
	}
}
