#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>

#include "game.h"

constexpr int MAX_MOVE_NUM = 128;

class MoveSort
{
private:
	int idx;
	int num;	// 着法数量
	Move moves[MAX_MOVE_NUM];	// 着法数组

	void sort();	// 排序
public:
	void getAllMoves(Game& game);	// 产生所有着法
	void getCapMoves(Game& game);
	
	int32_t next() {
		if (idx >= num)
			return 0;
		return this->moves[idx++].step;
	}

	int32_t random() {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 generator(seed);
		return this->moves[generator() % num].step;
	}
};

#endif