#include <string.h>
#include <iostream>
#include <Windows.h>

#include "pregen.h"
#include "game.h"
#include "evaluate.h"


/* �������Ǻ�Լ����
 *
 * sq: �������(��������0��255������"pregen.cpp")
 * pc: �������(��������0��47������"position.cpp")
 * pt: �����������(��������0��6������"position.cpp")
 * mv: �ŷ�(��������0��65535������"position.cpp")
 * sd: ���ӷ�(������0����췽��1����ڷ�)
 * vl: �����ֵ(��������"-MATE_VALUE"��"MATE_VALUE"������"position.cpp")
 * (ע����������Ǻſ���uc��dw�ȴ��������ļǺ����ʹ��)
 * pos: ����(PositionStruct���ͣ�����"position.h")
 * sms: λ�к�λ�е��ŷ�����Ԥ�ýṹ(����"pregen.h")
 * smv: λ�к�λ�е��ŷ��ж�Ԥ�ýṹ(����"pregen.h")
 */

Game::Game()
{
	// �������
	std::memset(this->board, 0, sizeof(this->board));
	std::memset(this->pieces, 0, sizeof(this->pieces));
	std::memset(this->bitCol, 0, sizeof(this->bitCol));
	std::memset(this->bitRow, 0, sizeof(this->bitRow));
	this->bitPieces = 0;
	// ��ʼ����Ϸ��ɫ
	this->cur_player = RED;
	this->red_val = this->black_val = 0;
}

void Game::initGame()
{
	// �������
	std::memset(this->board, 0, sizeof(this->board));
	std::memset(this->pieces, 0, sizeof(this->pieces));
	std::memset(this->bitCol, 0, sizeof(this->bitCol));
	std::memset(this->bitRow, 0, sizeof(this->bitRow));
	this->bitPieces = 0;
	// ��ʼ����Ϸ��ɫ
	this->cur_player = RED;
	this->red_val = this->black_val = 0;
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
	// ����λ�� λ��
	this->bitRow[getIdxRow(sq)] ^= preGen.bitRowMask[sq];
	this->bitCol[getIdxCol(sq)] ^= preGen.bitColMask[sq];
	this->bitPieces ^= 1 << (pc - 16);
	// ���¹�ֵ
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
	}
	// ����zobr��ֵ
}

void Game::changePlayer()
{
	this->cur_player = !this->cur_player;
}

void Game::buildFromFen(std::string fen)
{
	this->initGame();
	// ��������洢�������Ӷ�Ӧ�ı��
	uint8_t red_idx[7] = {JIANG_FROM, SHI_FROM, XIANG_FROM, 
							MA_FROM, JU_FROM, PAO_FROM, BING_FROM};
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
			// ����
			j = BOARD_LEFT;
			i++;
			if (i > BOARD_BOTTOM)
				break;
		}
		else if (isdigit(ch))
		{
			// �ո�
			j += (uint8_t)(ch - '0');
		}
		else if (ch >= 'A' && ch <= 'Z' && j <= BOARD_RIGHT)
		{
			// ����
			uint8_t temp = getChessNumber(ch);
			putChess(coordToPos(i, j), red_idx[temp]);
			red_idx[temp]++;
			j++;
		}
		else if (ch >= 'a' && ch <= 'z' && j <= BOARD_RIGHT)
		{
			// ����
			uint8_t temp = getChessNumber(ch + 'A' - 'a');
			putChess(coordToPos(i, j), black_idx[temp]);
			black_idx[temp]++;
			j++;
		}
		pos++;
	}
}

int Game::takeOneMove(int32_t move)
{
	int pt;
	int src = getSrc(move);
	int dst = getDst(move);
	int chessOnSrc = this->board[src];
	int chessOnDst = this->board[dst];
	
	// ������ֹλ��
	if (chessOnDst)
	{
		// Ŀ��λ������
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
		// ����zobr
		/////////////////////////////////////////////////
	}
	else
	{
		// û�г��� ����Ŀ��λ�õ�λ��λ��
		this->bitRow[getIdxRow(dst)] ^= preGen.bitRowMask[dst];
		this->bitCol[getIdxCol(dst)] ^= preGen.bitColMask[dst];
	}

	// ������ʼλ��
	this->board[src] = 0;
	this->board[dst] = chessOnSrc;
	this->pieces[chessOnSrc] = dst;
	this->bitRow[getIdxRow(src)] ^= preGen.bitColMask[src];
	this->bitCol[getIdxCol(src)] ^= preGen.bitColMask[src];
	pt = pieceType[chessOnSrc];
	if (chessOnSrc < 32)
	{
		this->red_val += normalEval.redPieces[pt][dst] - normalEval.redPieces[pt][src];
	}
	else
	{
		this->black_val += normalEval.blackPieces[pt][dst] - normalEval.blackPieces[pt][dst];
		pt += 7;
	}
	// ����zobr
	/////////////////////////////////////////////////
	return chessOnDst;
}

void Game::deleteOneMove(int32_t move, int captured)
{
	int src = getSrc(move);
	int dst = getDst(move);
	int piece = this->board[dst];

	this->board[src] = piece;
	this->pieces[piece] = src;
	this->bitRow[src >> 4] ^= preGen.bitRowMask[src];
	this->bitCol[src & 15] ^= preGen.bitColMask[src];

	if (captured > 0)
	{
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

void Game::printForDebug()
{
	std::cout << "_______________________________\n";
	// ��ӡ����
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
					std::cout << "��";
				}
				else if (temp >= 9) {
					std::cout << "��";
				}
				else if (temp >= 7) {
					std::cout << "��";
				}
				else if (temp >= 5) {
					std::cout << "��";
				}
				else if (temp >= 3) {
					std::cout << "��";
				}
				else if (temp >= 1) {
					std::cout << "��";
				}
				else if (temp >= 0) {
					std::cout << "��";
				}
			}
			else if (temp >= 16) {
				temp -= 16;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				if (temp >= 11) {
					std::cout << "��";
				}
				else if (temp >= 9) {
					std::cout << "��";
				}
				else if (temp >= 7) {
					std::cout << "��";
				}
				else if (temp >= 5) {
					std::cout << "��";
				}
				else if (temp >= 3) {
					std::cout << "��";
				}
				else if (temp >= 1) {
					std::cout << "ʿ";
				}
				else if (temp >= 0) {
					std::cout << "˧";
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
			}
			std::cout << " ";
		}
		std::cout << "|\n";
	}
	// 
	std::cout << "-------------------------------\n";
	std::cout << "   a  b  c  d  e  f  g  h  i\n";
}
