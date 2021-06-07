#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>

#include "game.h"

constexpr int MAX_MOVE_NUM = 128;
constexpr int TAKEN = -123456789;
constexpr int PHASE_HASH = 0;
constexpr int PHASE_GEN_CAP = 1;
constexpr int PHASE_GOOD_CAP = 2;
constexpr int PHASE_KILLER_1 = 3;
constexpr int PHASE_KILLER_2 = 4;
constexpr int PHASE_GEN_NONCAP = 5;
constexpr int PHASE_GOOD_NONCAP = 6;

class MoveSort
{
private:
	int idx = 0;
	int num = 0;	// 着法数量
	uint16_t mvHash = 0; 
	Move moves[MAX_MOVE_NUM] = {0};	// 着法数组
	uint16_t	killer_1 = 0, killer_2 = 0;	// 杀手着法
	void sort();	// 排序
public:
	int phase = 1;	// 着法生成阶段

	void getAllMoves(Game& game, uint16_t mvHash, uint16_t* killerTable = NULL);	// 产生所有着法
	void getQuiescAll(Game& game);
	void getCapMoves(Game& game);	// 产生所有吃子着法
	void updHistoryVal(); // 历史表启发
	
	uint16_t next(Game& game);

	// 为静态搜索生成下一个着法 略有不同
	uint16_t nextQuiesc(bool all = false) {
		if (idx >= num)
			return 0;
		if (!all && moves[idx].value <= 0)
			return 0;
		return moves[idx++].step;
	}
};

// 历史表
extern int nHistory[65536];

// 清空历史表
inline void clearHistory()
{
	for (int i = 0; i < 65536; i++) nHistory[i] = 0;
}

// 更新历史表与杀手着法
inline void updBest(uint16_t mv, int depth, uint16_t *killerTable)
{
	nHistory[mv] += depth * depth;
	if (killerTable[0] != mv)
	{
		killerTable[1] = killerTable[0];
		killerTable[0] = mv;
	}
}
#endif