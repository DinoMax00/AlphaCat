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
			if (commandVec.size() == 8)
			{
				std::cout << commandStr[2] << std::endl;
			}
			else
			{
				std::cout << commandVec[commandVec.size()-1] << std::endl;
			}
		}
		// 设定限时
		else if (commandVec[0] == "go" && commandVec[1]=="time")
		{
			std::cout << std::stoi(commandVec[2]) << std::endl;
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