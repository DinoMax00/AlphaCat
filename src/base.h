/*****************************************************************//**
 * \file   base.h
 * \brief  项目中使用到的常量、类型、宏定义
 * 
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/

#ifndef BASE_H
#define BASE_H

/// 棋子类型
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

/// 游戏结果，用于随机快速走子函数
typedef int GameStatus;
constexpr auto WIN = 1;
constexpr auto LOSS = -1;
constexpr auto TIE = 0;

/// 当前执子
typedef bool player_type;
constexpr auto RED_PLAYER = 1; ///< 红棋
constexpr auto BLACK_PLAYER = 0; ///< 黑棋

/// 一回合中预期最大走子方法数
constexpr auto MAX_MOVES = 80;

#endif