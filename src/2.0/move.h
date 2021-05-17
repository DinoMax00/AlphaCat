#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>

struct Move
{
	int32_t step;	// move类型
	uint8_t src;	// 起点
	uint8_t	dst;	// 终点
	uint8_t	captureed; // 被吃子
	uint8_t chk;	   // 着法是否将军
};
union CapMove
{
	Move mov;
	int val;
};


#endif
