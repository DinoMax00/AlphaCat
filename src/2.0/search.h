#ifndef SEARCH_H
#define SEARCH_H

#include "game.h"
#include "pregen.h"

const int LIMIT_DEPTH = 64;             // 搜索的极限深度
const int MATE_VALUE = 10000;           // 最高分值，即将死的分值
const int WIN_VALUE = MATE_VALUE - 200; // 搜索出胜负的分值界限，超出此值就说明已经搜索出杀棋了

// 搜索前可设置的全局变量，指定搜索参数
struct SearchStruct {
	Game pos;                   // 有待搜索的局面
	int result;					// 返回着法
};

// 搜索的启动过程
void searchMain(int depth);

extern SearchStruct Search;

#endif