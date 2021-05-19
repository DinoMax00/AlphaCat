#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>
#include <algorithm>

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
};

#endif