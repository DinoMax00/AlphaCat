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
	this->split();
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
		if (commandVec[0] == "isready")
		{
			std::cout << "readyok" << std::endl;
		}
		// 移子
		if (commandVec[0] == "position")
		{
			// std::cout << commandVec[3] << std::endl;
			if (commandVec.size() == 8 || commandVec.size() == 9)
			{
				std::cout << commandVec[2] << std::endl;
				board.buildBoardFromFen(commandVec[2]);
			}
			else
			{
				// std::cout << commandVec[commandVec.size()-1] << std::endl;
			}
			// board.printBoardForDebug();
		}
		// 走子
		else if (commandVec[0] == "go")
		{
			if (commandVec[1] == "time")
			{
				// std::cout << std::stoi(commandVec[2]) << std::endl;
			}
			std::cout << "bestmove " << "e0e1" << std::endl;
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