/*****************************************************************//**
 * \file   log.h
 * \brief  日志类头文件
 * 
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
#ifndef LOG_H
#define LOG_H


#include <string>
#include <fstream>

class Log
{
private:
	std::ofstream out;
	bool on = true;
	char path[20] = "./log.txt";
public:
	void clear();
	void baseMsg();
	template <typename T>
	void info(T msg);
	template <typename T>
	void add(T msg, bool end = true);
	void error(std::string msg);
};


template <typename T>
void Log::info(T msg)
{
	if (!on) return;
	baseMsg();
	out.open(this->path, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	if (out.is_open())
	{
		out << "INFO: " << msg << std::endl;
		out.close();
	}
	else out.close();
}


template <typename T>
void Log::add(T msg, bool end)
{
	if (!on) return;
	out.open(this->path, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	if (out.is_open())
	{
		out << msg;
		if (end)
			out << std::endl;
		out.close();
	}
	else out.close();
}


#endif  
