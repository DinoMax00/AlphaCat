#include "move.h"

#include <iostream>

int nHistory[65536];

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

void MoveSort::getCapMoves(Game& game)
{
	this->idx = 0;
	this->num = game.genCapMoves(this->moves);
	this->sort();
}
