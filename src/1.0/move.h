/*****************************************************************//**
 * @file   move.h
 * @brief  棋子移动类头文件
 * 
 * @author AlphaCat
 * @date   March 2021
 *********************************************************************/
#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>

#include "base.h"


/**
 * @brief 棋子移动类
 * 封装了棋子的一次走动
 * 
 */
class Move
{
public:
	/// 起始位置
	unsigned char from = 0;
	/// 目标位置
	unsigned char to = 0;
	/// 记录目标位置上的棋子，方便撤回操作
	unsigned char chessOnTo = EMPTY;
	/// 记录移动后的局面估值 用于排序
	int val = 0;
	   
	/**
	 * @brief 默认构造函数
	 * 
	 */
	Move();

	/**
	 * @brief 根据描述移动的字符串构造Move
	 * 
	 * @param move_str 长度为4的移动描述字符串
	 */
	Move(std::string move_str);

	/**
	 * @brief 通过始末位置构造Move
	 * 
	 * @param from 起始位置
	 * @param to 目标位置
	 */
	Move(int from, int to);

	/**
	 * @brief 将Move类描述的移动转为一个长度为4的字符串
	 * 
	 * @return std::string 移动描述字符串
	 */
	std::string moveToString();

	/**
	 * @brief 复制一个Move类
	 * 
	 * @param 要复制的对象
	 */
	void copyOneMove(Move& rhs);

	/**
	 * @brief 运算符重载
	 */
	bool operator < (const Move& x) const
	{
		return this->val > x.val;
	}
};


#endif 
