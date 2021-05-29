#ifndef SEARCH_H
#define SEARCH_H

#include <string.h>

#include "game.h"
#include "pregen.h"

const int LIMIT_DEPTH = 64;             // 搜索的极限深度
const int MATE_VALUE = 10000;           // 最高分值，即将死的分值
const int BAN_VALUE = MATE_VALUE - 100; // 长将判负分值
const int WIN_VALUE = MATE_VALUE - 200; // 搜索出胜负的分值界限，超出此值就说明已经搜索出杀棋了
const int NULL_DEPTH = 2;				// 空着剪裁的深度

struct SearchStruct {
	Game pos;                   // 有待搜索的局面
	int result = -1;			// 返回着法
	int nodes = 0;				// 搜索节点数量

	bool stop;					// 是否超时
	uint64_t cur_time = 0;			// 计时器	
	uint64_t time_limit = 2000;		// 限时

	uint16_t killeTable[LIMIT_DEPTH][2] = {0}; // 杀手着法

	void clearKiller()
	{
		memset(killeTable, 0, sizeof(killeTable));
	}
};

// 搜索的启动过程
void searchMain(int depth);

extern SearchStruct Search;

#endif