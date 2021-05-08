/*****************************************************************//**
 * \file   ucci.cpp
 * \brief  ucci协议源文件
 * 
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
#include <iostream>
#include <string>

#include "search.h"
#include "ucci.h"
#include "log.h"
#include "mcts.h"

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
		else if(tmp!="")
		{
			commandVec.push_back(tmp);
			tmp.clear();
		}
	}
}


void UcciEngine::getCommand()
{
	std::getline(std::cin, commandStr);
	if(commandStr.size()) split();
	Log().info("获得命令->"+commandStr);

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
			if (commandVec.size() <= 9)
				board.player = BLACK_PLAYER;
			else if ((commandVec.size() - 9) % 2)
				board.player = RED_PLAYER;
			else
				board.player = BLACK_PLAYER;
		}
		else
		{
			if (commandVec.size() <= 9)
				board.player = RED_PLAYER;
			else if ((commandVec.size() - 9) % 2)
				board.player = BLACK_PLAYER;
			else
				board.player = RED_PLAYER;
		}
	}
	else if (commandVec[1] == "startpos")
	{
		// 更新游戏角色
		if (commandVec.size() <= 3)
			board.player = RED_PLAYER;
		else if ((commandVec.size() - 3) % 2)
			board.player = BLACK_PLAYER;
		else
			board.player = RED_PLAYER;
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
			Log().info("引擎响应->readyok");
		}
		// 新的对局
		else if (commandVec[0] == "setoption")
		{
			if (commandVec[1] == "newgame")
			{
				board.buildBoardFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
			}
		}
		// 移子
		else if (commandVec[0] == "position")
		{
			updWhichPlayer();
			if (commandVec[1] == "fen")
			{
				// 更新最近吃子局面 
				board.buildBoardFromFen(commandVec[2]);
				// 更新后续局面
				if (commandVec.size() > 9)
				{
					for (int i = 9; i < (int)commandVec.size(); i++) {
						board.genOneMove(commandVec[i]);
					}
				}
			}
			else if (commandVec[1] == "startpos" && commandVec.size() > 3)
			{
				// 更新局面
				board.buildBoardFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
				// 更新后续局面
				if (commandVec.size() > 3)
				{
					for (int i = 3; i < commandVec.size(); i++) {
						board.genOneMove(commandVec[i]);
					}
				}
			}
			Log().info(std::string("当前游戏角色: ") + (board.player == RED_PLAYER ? "RED" : "BLACK"));
		}
		// 走子
		else if (commandVec[0] == "go" && commandVec.size()>2)
		{
			// board.printBoardForDebug();
			board.generateMoves();
			if (commandVec[1] == "time")
			{
				// 时间设定
				// std::cout << std::stoi(commandVec[2]) << std::endl;
			}
			// 获取响应
			searcher search;
			std::string  s = search.getBestMove(board).moveToString();
			// 测试走子
			// std::string s = board.randomRunMove().moveToString();
			if (s == "a0i9") {
				std::cout << "nobestmove" << std::endl;
				Log().info("引擎响应->nobestmove");
				continue;
			}
			std::cout << "bestmove " << s << std::endl;
			Log().info("引擎响应->bestmove " + s);
			// 更新棋盘
			board.genOneMove(s);
		}
		// 拜拜
		else if (commandVec[0] == "quit")
		{
			std::cout << "bye" << std::endl;
			Log().info("引擎响应->bye");
			return;
		}
		clear();
	}
}