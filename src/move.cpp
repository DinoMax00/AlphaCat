/*****************************************************************//**
 * \file   move.cpp
 * \brief  棋子移动类源文件
 * 
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
#include <iostream>
#include <string>

#include "move.h"
#include "base.h"
#include "board.h"

Move::Move()
{
}

std::string Move::moveToString()
{
	
	std::string move = "";
	move += char((from & 15) - 4 + 'a');
	move += char(12 - (from >> 4) + '0');
	move += char((to & 15) - 4 + 'a');
	move += char(12 - (to >> 4) + '0');
	return move;
}

Move::Move(int from, int to)
{
	this->from = from;
	this->to = to;
}


Move::Move(std::string move)
{
	from = ((12 + '0' - move[1]) << 4) + move[0] - 'a' + 4;
	to = ((12 + '0' - move[3]) << 4) + move[2] - 'a' + 4;
}

void Move::copyOneMove (Move& rhs)
{
	from = rhs.from;
	to = rhs.to;
}