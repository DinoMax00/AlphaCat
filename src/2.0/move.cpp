#include "move.h"

#include <iostream>

int nHistory[65536];

void MoveSort::sort()
{
	std::sort(this->moves + this->idx, this->moves + this->num);
}

void MoveSort::updHistoryVal()
{
	for (int i = this->idx; i < this->num; i++)
	{
		this->moves[i].value = nHistory[this->moves[i].step];
	}
}

void MoveSort::getAllMoves(Game& game, uint16_t * killerTable)
{
	this->phase = PHASE_GEN_CAP;
	this->killer_1 = this->killer_2 = 0;
	if (killerTable != NULL)
	{
		this->killer_1 = killerTable[0];
		this->killer_2 = killerTable[1];
	}
}

void MoveSort::getQuiescAll(Game& game)
{
	this->idx = 0;
	this->num = game.genAllMoves(this->moves);
	this->updHistoryVal();
	this->sort();
}

void MoveSort::getCapMoves(Game& game)
{
	this->idx = 0;
	this->num = game.genCapMoves(this->moves);
	this->sort();
}

uint16_t MoveSort::next(Game& game) {
	switch (this->phase)
	{
		// 生成吃子着法
		case PHASE_GEN_CAP:
			this->phase = PHASE_GOOD_CAP;
			this->idx = 0;
			this->num = game.genCapMoves(this->moves);
			this->sort();
		// MVV(LVA)启发
		case PHASE_GOOD_CAP:
			if (this->idx < this->num && this->moves[this->idx].value > 1) {
				this->moves[this->idx].value = -1e9;
				return this->moves[this->idx++].step;
			}
		// 杀手启发1
		case PHASE_KILLER_1:
			this->phase = PHASE_KILLER_2;
			if (this->killer_1 && game.legalMove(killer_1))
				return this->killer_1;
		// 杀手启发2
		case PHASE_KILLER_2:
			this->phase = PHASE_GEN_NONCAP;
			if (this->killer_2 && game.legalMove(killer_2))
				return this->killer_2;
		// 生成不吃子着法
		case PHASE_GEN_NONCAP:
			this->phase = PHASE_GOOD_NONCAP;
			this->num += game.genNonCapMoves(this->moves + this->num);
			this->updHistoryVal();
			this->idx = 0;
			this->sort();
		case PHASE_GOOD_NONCAP:
			if (this->idx < this->num) {
				return this->moves[this->idx++].step;
			}
		default:
			return 0;
	}
	if (idx >= num)
		return 0;
	return this->moves[idx++].step;
}