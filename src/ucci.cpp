#include <iostream>
#include <string>
#include <fstream>
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
			if (commandVec.size() == 8 || commandVec.size() == 9)
			{
				if (!moved)
				{
					if (commandVec[2] == "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR")
					{
						board.player = RED;
					}
					else
					{
						board.player = BLACK;
					}
					moved = true;
				}
				board.buildBoardFromFen(commandVec[2]);
			}
			else
			{
				if (!moved)
				{
					board.player = BLACK;
					moved = true;
				}
				board.genOneMove(commandVec[commandVec.size() - 1]);
			}
		}
		// 走子
		else if (commandVec[0] == "go")
		{
			board.generateMoves();
			if (commandVec[1] == "time")
			{
				// 时间设定
				// std::cout << std::stoi(commandVec[2]) << std::endl;
			}
			std::string  s = board.randomRunMove().moveToString();

			std::ofstream out;
			out.open("out.txt", std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
			if (out.is_open())
			{
				out << s << std::endl;
				out.close();
			}
			else out.close();

			if (s == "a0i9") {
				std::cout << "nobestmove" << std::endl;
				return;
			}
			std::cout << "bestmove " << s << std::endl;
			board.genOneMove(s);
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