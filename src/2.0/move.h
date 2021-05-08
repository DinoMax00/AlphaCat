#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>

struct Move
{
	uint8_t src;	// 起点
	uint8_t	dst;	// 终点
	int8_t	captureed; // 被吃子
};


#endif
