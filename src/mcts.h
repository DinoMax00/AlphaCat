/*****************************************************************/ /**
 * \file   mcts.h
 * \brief  ���ؿ�������ͷ�ļ�
 * 
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
#ifndef mcts_H
#define mcts_H

#include <string>
#include <vector>
#include "move.h"
#include "board.h"

const double MCTS_C = 0.5;
const unsigned int MCTS_TIMES = 15000;

/**
 * @brief 封装了蒙特卡洛树搜索方法
 * 
 */
class Mcts
{
public:
	/// 记录对应的棋盘
	Board *situation;
	/// 记录对应点下胜利次数
	unsigned int win_times;
	/// 记录对应点下模拟总次数
	unsigned int all_times;
	/// 用估值函数算出来的胜率
	double p_of_win;
	/// 用子结点胜率的和
	double point_all;
	/// 这个点的胜率是否已经确定
	bool is_over;
	/// 指向模拟开始的根节点
	Mcts *initial_mcts;
	/// 指向这个结点的父亲
	Mcts *father;
	/// 如果这个点的移动确定了，指向最好的下一步
	Mcts *best_move_after;
	/// 上一个局面到现在局面的Move
	Move from_move;
	/// 当前局面所有展开的子局面
	std::vector<Mcts *> tryed_choices;

	Mcts(Board *);

	void selectionOfTry();

	std::string getBestMoveString();
	void printFormctsDebug();
	void printFormctsDebug2();
	Mcts *new_world(Board *);

	//unsigned int draw_times;
	//Move& best_move;
	//void ExpansionOfNew();
	// Move& getBestMove();
};

#endif