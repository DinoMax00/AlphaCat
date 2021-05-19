#include "move.h"

#include <iostream>

void MoveSort::sort()
{
	std::sort(this->moves, this->moves + this->num);
}

void MoveSort::getAllMoves(Game& game)
{
	this->idx = 0;
	this->num = game.genAllMoves(this->moves);
	this->sort();
}