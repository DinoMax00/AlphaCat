#ifndef BOARD_H
#define BOARD_H

/*****************************************************************//**
 * \file   board.h
 * \brief  保存对局状态
 *
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
#include <stdint.h>
#include <string>

#include "move.h"

// 游戏角色
constexpr bool RED	= true;
constexpr bool BLACK = false;
// 棋盘定位 0x33-0xcb
constexpr uint8_t BOARD_LEFT = 3;
constexpr uint8_t BOARD_TOP = 3;
constexpr uint8_t BOARD_RIGHT = 11;
constexpr uint8_t BOARD_BOTTOM = 12;
// 各类棋子编号
constexpr uint8_t JIANG_FROM = 0;
constexpr uint8_t SHI_FROM = 1;
constexpr uint8_t SHI_TO = 2;
constexpr uint8_t XIANG_FROM = 3;
constexpr uint8_t XIANG_TO = 4;
constexpr uint8_t MA_FROM = 5;
constexpr uint8_t MA_TO = 6;
constexpr uint8_t JU_FROM = 7;
constexpr uint8_t JU_TO = 8;
constexpr uint8_t PAO_FROM = 9;
constexpr uint8_t PAO_TO = 10;
constexpr uint8_t BING_FROM = 11;
constexpr uint8_t BING_TO = 15;


class Game
{
private:
	void initGame();		// 初始化
	void putChess(int32_t sq, int32_t pc, bool del = false);		// 把一个棋子放在棋盘上，del为true则是移除棋子
public:
	bool cur_player = RED;	// 当前游戏角色
	uint8_t	board[256];		// 棋盘数组
	uint8_t	pieces[48];		// 每个棋子的位置
	int32_t	red_val;		// 红棋估值
	int32_t black_val;		// 黑棋估值

	// 方法
	Game();					// 默认构造函数 用于初始化
	void changePlayer();	// 对换角色
	
	void buildFromFen(std::string fen);	// 根据fen串构建棋盘

	void takeOneMove(int32_t move);		// 一步移动
	void deleteOneMove(int32_t move);	// 撤回一步移动

	// 着法生成
	void genAllMoves(Move moves[]);		// 生成所有着法
	void genCapMoves(Move moves[]);		// 生成吃子着法
	int genNotCapMoves(Move moves[]);	// 生成不吃子着法

	// 调试
	void printForDebug();	// 棋盘输出到终端
};


// 坐标转为数组索引
inline int coordToPos(int row, int col)
{
	return col + (row << 4);
}

// 获取棋子标号开始位置 红是16 黑是32
inline int sideTag(bool side)
{
	return side == RED ? 16 : 32;
}

// 根据移动信息 获取起点
inline int getSrc(int32_t move)
{
	return move & 255;
}

// 根据移动信息 获取终点
inline int getDst(int32_t move)
{
	return move >> 8;
}

// 获取位置的行信息
inline int getIdxRow(int idx)
{
	return idx >> 4;
}

// 获取位置的列信息 位运算取模
inline int getIdxCol(int idx)
{
	return idx & 15; 
}

// 起点终点组合成一个move类型
inline int32_t getMoveType(int src, int dst)
{
	return src + (dst << 8);
}

// move转字符串
inline std::string moveToString(int32_t move)
{
	std::string str;
	str += getIdxCol(getSrc(move)) - BOARD_LEFT + 'a';
	str += '9' - (getIdxRow(getSrc(move)) - BOARD_TOP);
	str += getIdxCol(getDst(move)) - BOARD_LEFT + 'a';
	str += '9' - (getIdxRow(getDst(move)) - BOARD_TOP);

	return str;
}

// 字符串转move
inline int32_t stringToMove(std::string str)
{
	int src = coordToPos('9' - str[1] + BOARD_TOP, str[0] - 'a' + BOARD_LEFT);
	int dst = coordToPos('9' - str[3] + BOARD_TOP, str[2] - 'a' + BOARD_LEFT);
	return getMoveType(src, dst);
}

// 获取棋子大写字母对应的数字
inline uint8_t getChessNumber(char ch)
{
	switch (ch)
	{
		case 'K':
			return 0;
		case 'A':
			return 1;
		case 'B':
		case 'E':
			return 2;
		case 'N':
		case 'H':
			return 3;
		case 'R':
			return 4;
		case 'C':
			return 5;
		case 'P':
			return 6;
		default:
			return 7;
	}
}

#endif
