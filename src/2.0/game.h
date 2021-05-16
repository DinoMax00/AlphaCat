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

// 各种棋子的屏蔽位
constexpr uint32_t JIANG_BITPIECE = 1 << JIANG_FROM;
constexpr uint32_t SHI_BITPIECE = (1 << SHI_FROM) | (1 << SHI_TO);
constexpr uint32_t XIANG_BITPIECE = (1 << XIANG_FROM) | (1 << XIANG_TO);
constexpr uint32_t MA_BITPIECE = (1 << MA_FROM) | (1 << MA_TO);
constexpr uint32_t JU_BITPIECE = (1 << JU_FROM) | (1 << JU_TO);
constexpr uint32_t PAO_BITPIECE = (1 << PAO_FROM) | (1 << PAO_TO);
constexpr uint32_t BING_BITPIECE = (1 << BING_FROM) | (1 << (BING_FROM + 1)) |
    (1 << (BING_FROM + 2)) | (1 << (BING_FROM + 3)) | (1 << BING_TO);
constexpr uint32_t ATTACK_BITPIECE = MA_BITPIECE | JU_BITPIECE | PAO_BITPIECE | BING_BITPIECE;

/* 棋子序号对应的棋子类型
 *
 * 棋子序号从0到47，其中0到15不用，16到31表示红子，32到47表示黑子。
 * 每方的棋子顺序依次是：帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
 * 提示：判断棋子是红子用"pc < 32"，黑子用"pc >= 32"
 */
const int pieceType[48] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
};

class Game
{
private:
	void initGame();		// 初始化
	void putChess(int32_t sq, int32_t pc, bool del = false);		// 把一个棋子放在棋盘上，del为true则是移除棋子
public:
	bool cur_player = RED;	// 当前游戏角色
	int32_t	red_val;		// 红棋估值
	int32_t black_val;		// 黑棋估值
	union {
    uint32_t bitPieces;		// 16-32号棋子是否在棋盘上 ????16-47吧
	uint16_t splited_bitPieces[2];// 0为红色方的棋子，1为黑色方的棋子
	};
		

	uint8_t	board[256];		// 棋盘数组
	uint8_t	pieces[48];		// 每个棋子的位置
	uint16_t bitRow[16];	// 位行
	uint16_t bitCol[16];	// 位列

	

	// 方法
	Game();					// 默认构造函数 用于初始化
	void changePlayer();	// 对换角色
	
	void buildFromFen(std::string fen);	// 根据fen串构建棋盘

	void takeOneMove(int32_t move);		// 一步移动
	void deleteOneMove(int32_t move);	// 撤回一步移动

	// 着法生成
	int genAllMoves(Move moves[]);		// 生成所有着法
	int genCapMoves(Move moves[]);		// 生成吃子着法
	int genNonCapMoves(Move moves[]);	// 生成不吃子着法

	// 调试
	void printForDebug();	// 棋盘输出到终端

	// 根据子力局面评估
	void evaBoard();
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

// 双方的棋子
inline uint32_t bothBitpiece(uint32_t bitPiece) {
  return bitPiece + (bitPiece << 16);
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
