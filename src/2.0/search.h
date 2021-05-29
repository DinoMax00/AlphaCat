#ifndef SEARCH_H
#define SEARCH_H

#include <string.h>

#include "game.h"
#include "pregen.h"

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