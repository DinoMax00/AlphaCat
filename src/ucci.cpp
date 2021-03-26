#include <iostream>
#include <string>

#include "ucci.h"


void UcciEngine::clear()
{
	this->commandStr.clear();
	this->commandVec.clear();
}


void UcciEngine::split()
{
	commandStr += ' ';
	std::string tmp;
	for (auto ch : this->commandStr)
	{
		if (ch != ' ')
		{
			tmp += ch;
		}
		else if(tmp!="")
		{
			this->commandVec.push_back(tmp);
			tmp.clear();
		}
	}
}


void UcciEngine::getCommand()
{
	std::getline(std::cin, this->commandStr);
	if(commandStr.size()) this->split();
}


void UcciEngine::bootEngine()
{
	while (true)
	{
		getCommand();
		if (commandVec[0] == "ucci")
		{
			std::cout << "ucciok" << std::endl;
			this->clear();
			return;
		}
		this->clear();
	}
}


void UcciEngine::run()
{
	while (true)
	{
		getCommand();
		// 检测引擎是不是还活着
		if (!commandVec.size())
		{
			continue;
		}
		if (commandVec[0] == "isready")
		{
			std::cout << "readyok" << std::endl;
		}
		// 移子
		if (commandVec[0] == "position")
		{
			// std::cout << commandVec[3] << std::endl;
			this->side =  1;
			if (commandVec.size() == 8 || commandVec.size() == 9)
			{
				if (!moved)
				{
					if (commandVec[2] == "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR")
					{
						side = 1;
					}
					else
					{
						side = 0;
					}
					moved = true;
				}
				board.buildBoardFromFen(commandVec[2]);
			}
			else
			{
				if (!moved)
				{
					side = 0;
					moved = true;
				}
				board.genOneMove(commandVec[commandVec.size() - 1]);
			}
			// board.printBoardForDebug();
		}
		// 走子
		else if (commandVec[0] == "go")
		{
			board.generateMoves(side);
			if (commandVec[1] == "time")
			{
				// std::cout << std::stoi(commandVec[2]) << std::endl;
			}
			std::cout << "bestmove " << board.mov[0].moveToString() << std::endl;
			board.genOneMove(board.mov[0].moveToString());
			board.mov.clear();
		}
		// 拜拜
		else if (commandVec[0] == "quit")
		{
			std::cout << "bye" << std::endl;
			return;
		}
		this->clear();
	}
}