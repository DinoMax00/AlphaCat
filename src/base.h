#ifndef BASE_H
#define BASE_H

// 与游戏有关的基本参数

enum Chess
{
	EMPTY = 0,
	BOUNDARY = 255,
	R_JU = 1, B_JU = 101,
	R_MA = 2, B_MA = 102,
	R_XIANG = 3, B_XIANG = 103,
	R_SHI = 4, B_SHI = 104,
	R_JIANG = 5, B_JIANG = 105,
	R_PAO = 6, B_PAO = 106,
	R_BING = 7, B_BING = 107
};

enum Player
{
	RED = 1,
	BLACK = 0
};

enum Getall
{
	GET_ALL = 1,
	NOT_GET_ALL = 0
};

const int MAX_MOVES = 300;
#endif