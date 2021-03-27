#ifndef LOG_H
#define LOG_H


#include <string>
#include <fstream>

class Log
{
private:
	std::ofstream out;
	char path[20] = "./log.txt";
public:
	void clear();
	void baseMsg();
	void info(std::string msg);
	void error(std::string msg);
};

#endif  
