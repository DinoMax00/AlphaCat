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
		// 检测引擎是不是还活着
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
			// Search.pos.buildFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
			// position startpos moves f0e1 b9c7 c0e2 h9g7 h0g2 a9a8 g3g4 a8d8 b2c2 c6c5 c2c5 c7d5 c5c4 i9i8 b0c2 i8f8 a0b0 b7e7 i3i4 f8f3 i4i5 f3g3 g2f0 i6i5 i0i5 e7e3 c4d4 d5c7 c3c4 e3e4 i5i6 g3c3 i6g6 g7e8 d4d2 c3c2 d2d9 c2c4 d9f9 e9f9 h2f2 g9e7 g6f6 f9e9 f6f7 h7g7 f2g2 g7g2 f0g2 e8g9 f7f4 c7d5 f4f5 g9h7 e0f0 c4c3 b0b9 d8f8 f5f8 h7f8 g4g5 e7g5 g2h4 e4e1 d0e1 c3a3 b9c9 f8d9 h4f5 a3e3 f5g3 e3g3 e2g4 g3b3 c9c6 b3b0 f0f1 d9b8 c6a6 b8c6 a6a7 c6b4 a7a6 d5c3 a6e6 g5e7 f1f2 c3e4 f2e2 e4c5 e2d2 b4d3 e6d6 d3b2 g4e2 c5a4 e2g4 a4c3 g0i2
			Search.pos.printForDebug();
			std::cout << GetTickCount64() << std::endl;
			searchMain(20);
			std::cout << GetTickCount64() << std::endl;
			if (Search.result == -1) {
				std::cout << "nobestmove " << std::endl;
			}
			else {
				std::cout << "bestmove " << moveToString(Search.result) << std::endl;
			}
		}
		// 拜拜
		else if (commandVec[0] == "quit")
		{
			std::cout << "bye" << std::endl;
			return;
		}
		clear();
	}
}
