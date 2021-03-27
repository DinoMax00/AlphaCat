#include <iostream>
#include <fstream>
#include <string>

#include "log.h"

void Log::clear()
{
	out.open(this->path, std::ios::out | std::ios::binary);
	if (out.is_open())
	{
		out << __DATE__ << "--" << __TIME__ << ":\n";
		out << "��������" << std::endl;
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

void Log::info(std::string msg)
{
	baseMsg();
	out.open(this->path, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	if (out.is_open())
	{
		out << "INFO: " << msg << std::endl;
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