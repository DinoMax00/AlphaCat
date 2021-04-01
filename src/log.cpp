/*****************************************************************//**
 * \file   log.cpp
 * \brief  日志类源文件
 * 
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>
#include "log.h"

void Log::clear()
{
	out.open(this->path, std::ios::out | std::ios::binary);
	if (out.is_open())
	{
		out << __DATE__ << "--" << __TIME__ << ":\n";
		out << "程序启动" << std::endl;
		out.close();
	}
	else out.close();
}


void Log::baseMsg()
{
	out.open(this->path, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	if (out.is_open())
	{
		out << __DATE__ << "--" << __TIME__ << ":\n";
		out.close();
	}
	else out.close();
}

void Log::error(std::string msg)
{
	baseMsg();
	out.open(this->path, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	if (out.is_open())
	{
		out << "ERROR: " << msg << std::endl;
		out.close();
	}
	else out.close();
}