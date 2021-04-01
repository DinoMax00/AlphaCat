/*****************************************************************//**
 * @file   base.h
 * @brief  项目中使用到的常量、类型、宏定义
 * 
 * @author AlphaCat
 * @date   March 2021
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

/// 马移动位置
const int MA_Feasible[8] = { 0x21,0x1f,0x12,0x0e,-0x21,-0x1f,-0x12,-0x0e };
/// 马别腿位置
const int MA_Leg[8] = { 0x10,0x10,0x01,-0x01,-0x10,-0x10,-0x01,0x01 };
/// 相移动位置
const int XIANG_Feasible[4] = { 0x22,0x1e,-0x22,-0x1e };
/// 士移动位置
const int SHI_Feasible[4] = { 0x11,0x0f,-0x11,-0x0f };
/// 将移动位置
const int JIANG_Feasible[4] = { 0x10,0x01,-0x10,-0x01 };
/// 兵移动位置
const int BING_R_Feasible[3] = { 0x01,-0x01,-0x10 };
/// 卒移动位置
const int BING_B_Feasible[3] = { 0x01,-0x01,0x10 };

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