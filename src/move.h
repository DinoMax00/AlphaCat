#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>

class Move
{
private:
	unsigned char from;
	unsigned char to;
public:
	Move(std::string);
	Move(int, int);
	std::string moveToString();
};

#endif 
