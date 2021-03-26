#ifndef BOARD_H
#define BOARD_H



#include <string>
#include <vector>

#include "move.h"
/********************************************************
类名称：Board
功能：通过16*16的棋盘来表示棋盘，以减小9*10的棋盘的除法对效率影响
周围放置哨兵防止大量的边界检测
********************************************************/
class Board
{
public:
	//成员变量
	unsigned char player;			//当前走棋玩家
	unsigned char board[256];		//棋盘上某一位上的棋子
	unsigned char pos_of_king[2];	//棋盘上将帅的位置
	int num_of_move[8];				//每一种棋子的走棋方案数
	int num_of_chess;				//当前棋盘的棋子个数
	int num_of_rounds;				//回合数
	int num_of_no_eat_rounds;		//未吃子的回合数
	long long zobrist;				//当前棋盘的Zobrist键值

	std::vector<Move> mov;
	
	//成员函数
	/****************************************
	名称：Board()
	功能：默认构造函数
	参数：无
	返回值：构造的Board对象
	****************************************/
	Board();

	/****************************************
	名称：Board()
	功能：读取FEN格式来初始化对象的构造函数
	参数：const char* fen，构造当前局面的FEN格式串
	返回值：构造的Board对象
	****************************************/
	Board(std::string fen);

	/****************************************
	名称：buildBoardFromFen()
	功能：通过FEN数组来初始化对象
	参数：string fen，构造当前局面的FEN格式串
	返回值：构造的Board对象
	****************************************/
	void buildBoardFromFen(std::string fen);

	/****************************************
	名称：genOneMove()
	功能：通过move数组来移动
	参数：string move，构造当前局面的移动串
	返回值：
	****************************************/
	void genOneMove(std::string move);

	/****************************************
	名称：printBoardForDebug()
	功能：打印棋盘看查情况
	参数：
	返回值：
	****************************************/
	void printBoardForDebug();
	bool judgeBorder(unsigned char dest);
	void generateMoves();
};

const int MA_Feasible[8] = { 0x21,0x1f,0x12,0x0e,-0x21,-0x1f,-0x12,-0x0e };
const int MA_Leg[8] = { 0x10,0x10,0x01,0x01,-0x10,-0x10,-0x01,-0x01 };
const int XIANG_Feasible[4] = { 0x22,0x1e,-0x22,-0x1e };
const int SHI_Feasible[4] = { 0x11,0x0f,-0x11,-0x0f };
const int JIANG_Feasible[4] = { 0x10,0x01,-0x10,-0x01 };
const int BING_R_Feasible[3] = { 0x01,-0x01,-0x10 };
const int BING_B_Feasible[3] = { 0x01,-0x01,0x10 };
#endif