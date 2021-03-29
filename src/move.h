#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>

#include "base.h"

class Move
{
public:
	unsigned char from = 0;
	unsigned char to = 0;
	unsigned char chessOnTo = EMPTY;
	   
	Move();
	Move(std::string);
	Move(int, int);
	std::string moveToString();
	void copyOneMove(Move&);
};

const int MA_Feasible[8] = { 0x21,0x1f,0x12,0x0e,-0x21,-0x1f,-0x12,-0x0e };
const int MA_Leg[8] = { 0x10,0x10,0x01,-0x01,-0x10,-0x10,-0x01,0x01 };
const int XIANG_Feasible[4] = { 0x22,0x1e,-0x22,-0x1e };
const int SHI_Feasible[4] = { 0x11,0x0f,-0x11,-0x0f };
const int JIANG_Feasible[4] = { 0x10,0x01,-0x10,-0x01 };
const int BING_R_Feasible[3] = { 0x01,-0x01,-0x10 };
const int BING_B_Feasible[3] = { 0x01,-0x01,0x10 };

#endif 
