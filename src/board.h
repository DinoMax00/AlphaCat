/*****************************************************************//**
 * @file   board.h
 * @brief  棋盘类头文件
 * 
 * @author  AlphaCat
 * @date   March 2021
 *********************************************************************/
#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>

#include "base.h"
#include "move.h"
#include "hash.h"


/**
 * @brief 棋盘类
 * 通过16*16的棋盘来表示棋盘，以减小9*10的棋盘的除法对效率影响周围放置哨兵进行越界检测
 */
class Board
{
private:

public:
	player_type player = RED_PLAYER;		///< 当前走棋玩家
	unsigned char board[256];				///< 一维棋盘数组
	unsigned char pos_of_kings[2];			///< 棋盘上将帅的位置，索引直接传RED/BLACK_PLAYER即可
	int redValue;							///< 红棋子力
	int blackValue;							///< 黑棋子力
	int gameVal;                            ///< 局面子力，gameVal = player_value - oppsite_Value
	long long Zobrist = 0;						///< 当前局面的Zobrist键值
	Hash hashNum;							///< 生成的Hash数组

	std::vector<Move> move_vec;					///< 记录当前棋盘所有可行的走法
	
	/**
	* @brief 无参构造函数
	* @param 形参 参数说明
	*/
	Board();

	/**
	* @brief fen串解析构造函数
	* @param fen 棋局fen串
	*/
	Board(std::string fen);

	/**
	* @brief 由已有棋盘生成一个走指定一步之后的局面棋盘
	* @param board_pre 之前的棋盘
	* @param move 一步移动
	*/
	Board(Board* board_pre, Move& move);

	/**
	 * @brief 通过fen串更改棋盘局面
	 * @param fen fen串
	 */
	void buildBoardFromFen(std::string fen);

	/**
	 * @brief 由一个长度为4的移动命令对棋盘进行一步移动
	 * @param move 一步移动
	 */
	void genOneMove(std::string& move);

	/**
	 * @brief 由一个move类对棋盘进行一步移动
	 * @param move 一步移动
	 */
	void genOneMove(Move& move);

	/**
	 * @brief 输出棋盘到终端
	 */
	void printBoardForDebug();

	/**
	 * @brief 输出棋盘到日志
	 */
	void printBoardForDebug2();

	/**
	 * @brief 走法生成
	 * 
	 */
	void generateMoves();
	void generateMoves(player_type);

	/**
	 * @brief 在棋盘上撤销一步移动
	 * @param move_pre 之前采取的移动
	 */
	void deleteOneMove(Move& move_pre);


	/**
	 * @brief 随机走一步，用于引擎测试
	 * @return 一步合法的走子
	 */
	Move randomRunMove();

	/**
	 * @brief 检测一步棋走后是否还会被将军，用于合法走子判断
	 * 
	 * @param move 走法
	 * @return true表示走后还会被将军
	 */
	bool checkJiang(Move& move);

	/**
	 * @brief 检测指定一方一步棋走后是否还会被将军，用于合法走子判断
	 * 
	 * @param move 走法
	 * @param side 要检测的玩家
	 * @return true表示走后还会被将军
	 */
	bool checkJiang(Move& move, player_type side);

	/**
	 * @brief 为蒙特卡洛生成一盘随机走子
	 * 
	 * @return 游戏结果 当为胜时返回回合数
	 */
	GameStatus mctsMove();

	/**
	 * @brief 更新并返回局面估值函数
	 */
	int getGameVal();
};


#endif