#include <string.h>
#include <iostream>
#include <Windows.h>

#include "pregen.h"
#include "game.h"
#include "evaluate.h"


/* 匈牙利记号约定：
 *
 * sq: 格子序号(整数，从0到255，参阅"pregen.cpp")
 * pc: 棋子序号(整数，从0到47，参阅"position.cpp")
 * pt: 棋子类型序号(整数，从0到6，参阅"position.cpp")
 * mv: 着法(整数，从0到65535，参阅"position.cpp")
 * sd: 走子方(整数，0代表红方，1代表黑方)
 * vl: 局面价值(整数，从"-MATE_VALUE"到"MATE_VALUE"，参阅"position.cpp")
 * (注：以上五个记号可与uc、dw等代表整数的记号配合使用)
 * pos: 局面(PositionStruct类型，参阅"position.h")
 * sms: 位行和位列的着法生成预置结构(参阅"pregen.h")
 * smv: 位行和位列的着法判断预置结构(参阅"pregen.h")
 */

Game::Game()
{
	this->initGame();
}

void Game::initGame()
{
	// 清空棋盘
	std::memset(this->board, 0, sizeof(this->board));
	std::memset(this->pieces, 0, sizeof(this->pieces));
	std::memset(this->bitCol, 0, sizeof(this->bitCol));
	std::memset(this->bitRow, 0, sizeof(this->bitRow));
	this->zobrist = 0;
	this->zobrist_mirror = 0;
	this->bitPieces = 0;
	this->red_val = this->black_val = 0;
	this->move_num = 0;
	this->depth = 0;
}

void Game::putChess(int32_t sq, int32_t pc, bool del)
{
	if (del)
	{
		this->board[sq] = 0;
		this->pieces[sq] = 0;
	}
	else
	{
		this->board[sq] = pc;
		this->pieces[pc] = sq;
	}
	// 更新位行 位列
	this->bitRow[getIdxRow(sq)] ^= preGen.bitRowMask[sq];
	this->bitCol[getIdxCol(sq)] ^= preGen.bitColMask[sq];
	this->bitPieces ^= 1 << (pc - 16);
	// 更新估值
	int pt = pieceType[pc];
	if (pc < 32)
	{
		if (del)
		{
			this->red_val -= normalEval.redPieces[pt][sq];
		}
		else
		{
			this->red_val += normalEval.redPieces[pt][sq];
		}
	}
	else
	{
		if (del)
		{
			this->black_val -= normalEval.blackPieces[pt][sq];
		}
		else
		{
			this->black_val += normalEval.blackPieces[pt][sq];
		}
		pt += 7;
	}
	// 更新zobr键值
	zobrist ^= ZobrTable[pt][sq];
	zobrist_mirror ^= ZobrTable[pt][sq + 14 - 2 * (sq % 16)];
}

inline void Game::changePlayer()
{
	this->cur_player = !this->cur_player;
	zobrist ^= ZobrPlayer;
	zobrist_mirror ^= ZobrPlayer;
}

void Game::buildFromFen(std::string fen)
{
	this->initGame();
	// 两个数组存储各类棋子对应的标号
	uint8_t red_idx[7] = { JIANG_FROM, SHI_FROM, XIANG_FROM,
							MA_FROM, JU_FROM, PAO_FROM, BING_FROM };
	uint8_t black_idx[7] = { JIANG_FROM, SHI_FROM, XIANG_FROM,
							MA_FROM, JU_FROM, PAO_FROM, BING_FROM };
	for (int i = 0; i < 7; i++)
	{
		red_idx[i] += 16;
		black_idx[i] += 32;
	}

	uint8_t pos = 0;
	uint8_t i = BOARD_TOP, j = BOARD_LEFT;
	uint8_t len = (uint8_t)fen.length();
	while (pos < len)
	{
		char ch = fen[pos];
		if (ch == '/')
		{
			// 换行
			j = BOARD_LEFT;
			i++;
			if (i > BOARD_BOTTOM)
				break;
		}
		else if (isdigit(ch))
		{
			// 空格
			j += (uint8_t)(ch - '0');
		}
		else if (ch >= 'A' && ch <= 'Z' && j <= BOARD_RIGHT)
		{
			// 红棋
			uint8_t temp = getChessNumber(ch);
			putChess(coordToPos(i, j), red_idx[temp]);
			red_idx[temp]++;
			j++;
		}
		else if (ch >= 'a' && ch <= 'z' && j <= BOARD_RIGHT)
		{
			// 黑棋
			uint8_t temp = getChessNumber(ch + 'A' - 'a');
			putChess(coordToPos(i, j), black_idx[temp]);
			black_idx[temp]++;
			j++;
		}
		pos++;
	}
	if (cur_player == RED)
	{
		zobrist ^= ZobrPlayer;
		zobrist_mirror ^= ZobrPlayer;
	}
}

int Game::moveChess(uint16_t mv)
{
	int pt;
	int src = getSrc(mv);
	int dst = getDst(mv);
	int chessOnSrc = this->board[src];
	int chessOnDst = this->board[dst];
	// 更新终止位置
	if (chessOnDst)
	{
		// 目标位置有子
		this->pieces[chessOnDst] = 0;
		this->bitPieces ^= 1 << (chessOnDst - 16);
		pt = pieceType[chessOnDst];
		if (chessOnDst < 32)
		{
			this->red_val -= normalEval.redPieces[pt][dst];
		}
		else
		{
			this->black_val -= normalEval.blackPieces[pt][dst];
			pt += 7;
		}
		// 更新zobr
		zobrist ^= ZobrTable[pt][dst];
		zobrist_mirror ^= ZobrTable[pt][dst + 14 - 2 * (dst % 16)];
	}
	else
	{
		// 没有吃子 更新目标位置的位行位列
		this->bitRow[getIdxRow(dst)] ^= preGen.bitRowMask[dst];
		this->bitCol[getIdxCol(dst)] ^= preGen.bitColMask[dst];
	}

	// 更新起始位置
	this->board[src] = 0;
	this->board[dst] = chessOnSrc;
	this->pieces[chessOnSrc] = dst;
	this->bitRow[getIdxRow(src)] ^= preGen.bitRowMask[src];
	this->bitCol[getIdxCol(src)] ^= preGen.bitColMask[src];
	pt = pieceType[chessOnSrc];
	if (chessOnSrc < 32)
	{
		this->red_val += normalEval.redPieces[pt][dst] - normalEval.redPieces[pt][src];
	}
	else
	{
		this->black_val += normalEval.blackPieces[pt][dst] - normalEval.blackPieces[pt][src];
		pt += 7;
	}
	// 更新zobr
	zobrist ^= ZobrTable[pt][src];
	zobrist_mirror ^= ZobrTable[pt][src + 14 - 2 * (src % 16)];
	zobrist ^= ZobrTable[pt][dst];
	zobrist_mirror ^= ZobrTable[pt][dst + 14 - 2 * (dst % 16)];
	return chessOnDst;
}

void Game::deleteMoveChess(uint16_t mv, int captured)
{
	int src = getSrc(mv);
	int dst = getDst(mv);
	int piece = this->board[dst];
	int pt = pieceType[piece];

	this->board[src] = piece;
	this->pieces[piece] = src;
	this->bitRow[src >> 4] ^= preGen.bitRowMask[src];
	this->bitCol[src & 15] ^= preGen.bitColMask[src];

	if (captured > 0)
	{
		pt = pieceType[captured];
		this->board[dst] = captured;
		this->pieces[captured] = dst;
		this->bitPieces ^= 1 << (captured - 16);
	}
	else
	{
		this->board[dst] = 0;
		this->bitRow[getIdxRow(dst)] ^= preGen.bitRowMask[dst];
		this->bitCol[getIdxCol(dst)] ^= preGen.bitColMask[dst];
	}
}

void Game::pushMove()
{
	MoveStack* p = this->moveStack + this->move_num;
	p->zobrist = this->zobrist;
	p->red_val = this->red_val;
	p->black_val = this->black_val;
}

void Game::popBack()
{
	MoveStack* p = this->moveStack + this->move_num;
	this->zobrist = p->zobrist;
	this->red_val = p->red_val;
	this->black_val = p->black_val;
}

bool Game::takeOneMove(uint16_t mv)
{
	if (this->move_num >= STACK_SIZE)
		return false;

	// zobr
	uint64_t oldzobr = this->zobrist;
	this->pushMove();

	int sq = getSrc(mv);
	int captured = moveChess(mv);
	// 看看是不是将军
	if (detectCheck(true) > 0)
	{
		deleteMoveChess(mv, captured);
		popBack();
		return false;
	}

	// 换边
	changePlayer();

	// 记录历史着法
	MoveStack* p = this->moveStack + this->move_num;
	p->move.step = mv;
	p->move.ChkChs = detectCheck();

	// 和棋着法
	if (captured)
	{
		p->move.CptDrw = captured;
	}
	else
	{
#ifdef CHASE_CHECK
		p->move.CptDrw = 0;
		/*
		if (p->move.ChkChs == 0)
			p->move.ChkChs = -chasedBy(mv);
		if (this->lastMove().CptDrw == -100)
			p->move.CptDrw = -100;
		else
			p->move.CptDrw = min(0, this->lastMove().CptDrw) - (p->move.ChkChs > 0 || this->lastMove().ChkChs > 0 ? 0 : 1);
		*/
#else 
		p->move.CptDrw = 0;
#endif // chase check
	}

	this->move_num++;
	this->depth++;
	return true;
}

void Game::deleteOneMove()
{
	this->move_num--;
	this->depth--;
	MoveStack* p = this->moveStack + this->move_num;
	int sq = getSrc(p->move.step);
	// 撤回
	deleteMoveChess(p->move.step, p->move.CptDrw);
	this->cur_player = !this->cur_player;
	popBack();
}

void Game::printForDebug()
{/*
	std::cout << "_______________________________\n";
	// 打印棋盘
	int row = 9;
	for (int i = 0x33; i <= 0xcb; i += 16)
	{
		std::cout << row-- << " |";
		for (int j = i; j <= i + 8; j++)
		{
			int temp = this->board[j];
			if (temp == 0) std::cout << "* ";
			else if (temp >= 32) {
				temp -= 32;
				if (temp >= 11) {
					std::cout << "兵";
				}
				else if (temp >= 9) {
					std::cout << "炮";
				}
				else if (temp >= 7) {
					std::cout << "车";
				}
				else if (temp >= 5) {
					std::cout << "马";
				}
				else if (temp >= 3) {
					std::cout << "象";
				}
				else if (temp >= 1) {
					std::cout << "仕";
				}
				else if (temp >= 0) {
					std::cout << "将";
				}
			}
			else if (temp >= 16) {
				temp -= 16;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				if (temp >= 11) {
					std::cout << "兵";
				}
				else if (temp >= 9) {
					std::cout << "炮";
				}
				else if (temp >= 7) {
					std::cout << "车";
				}
				else if (temp >= 5) {
					std::cout << "马";
				}
				else if (temp >= 3) {
					std::cout << "相";
				}
				else if (temp >= 1) {
					std::cout << "士";
				}
				else if (temp >= 0) {
					std::cout << "帅";
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
			}
			std::cout << " ";
		}
		std::cout << "|\n";
	}
	// 
	std::cout << "-------------------------------\n";
	std::cout << "   a  b  c  d  e  f  g  h  i   \n";
	*/
}

Move Game::lastMove()
{
	if (move_num == 0) return { 0, 0, 0, 0 };
	return this->moveStack[move_num - 1].move;
}

int Game::detectCircle(int recur)
{
	bool side = !this->cur_player;
	bool check, oppcheck;	// 长将标记

	check = oppcheck = true;

	MoveStack* stackpos = moveStack + move_num - 1;
	while (stackpos->move.step != 0 && stackpos->move.CptDrw <= 0)
	{
		if (side == this->cur_player)
		{
			check = check && (stackpos->move.ChkChs > 0);
			if (stackpos->zobrist == this->zobrist)
			{
				recur--;
				if (recur == 0)
				{
					return 1 + (check ? 2 : 0) + (oppcheck ? 4 : 0);
				}
			}
		}
		else
		{
			oppcheck = oppcheck && (stackpos->move.ChkChs > 0);
		}

		side = !side;
		stackpos--;
	}
	return 0;
}

int Game::detectCircle2(int recur)
{
	bool side = !this->cur_player;
	int32_t check = 0x1ffff; // 0x10000表示长将 0x00000~0x0ffff表示长捉
	int32_t opp_check = 0x1ffff;
	int32_t chk;

	MoveStack* stackpos = moveStack + move_num - 1;
	while (stackpos->move.step != 0 && stackpos->move.CptDrw <= 0)
	{
		chk = stackpos->move.ChkChs;
		if (side == this->cur_player)
		{
			if (chk == 0) check = 0;
			else if (chk > 0) check &= 0x10000;
			else check &= (1 << (-chk));

			if (stackpos->zobrist == this->zobrist)
			{
				recur--;
				if (recur == 0)
				{
					check = (check & 0xffff) ? 0xffff : check;
					opp_check = (opp_check & 0xffff) ? 0xffff : opp_check;
					return (check > opp_check) ? 3 : ((check < opp_check) ? 5 : (check ? 7 : 1));
				}
			}
		}
		else
		{
			if (chk == 0) opp_check = 0;
			else if (chk > 0) opp_check &= 0x10000;
			else opp_check &= (1 << (-chk));
		}

		side = !side;
		stackpos--;
	}
	return 0;
}

inline int Game::drawValue()
{
	return (depth & 1) ? DRAW_VALUE : -DRAW_VALUE;
}

int Game::circleVal(int rep)
{
	int val = (rep & 2) ? depth - BAN_VALUE : 0;
	val += (rep & 4) ? BAN_VALUE - depth : 0;
	return val ? val : drawValue();
}

bool Game::nullOk()
{
	return (cur_player == RED ? red_val : black_val) > 200;
}

bool Game::nullSafe()
{
	return (cur_player == RED ? red_val : black_val) > 400;
}

void Game::nullMove()
{
	pushMove();
	changePlayer();
	this->moveStack[move_num].move.step = 0;
	this->move_num++;
	this->depth++;
}

void Game::deleteNullMove()
{
	this->move_num--;
	this->depth--;
	this->cur_player = !this->cur_player;
	popBack();
}

bool Game::isDraw()
{
	return ((bitPieces & ATTACK_PIECES) == 0) || move_num >= STACK_SIZE;
}

