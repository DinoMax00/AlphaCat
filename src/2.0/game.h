#ifndef BOARD_H
#define BOARD_H

/*****************************************************************//**
 * \file   board.h
 * \brief  ����Ծ�״̬
 *
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
#include <stdint.h>
#include <string>

#include "move.h"

// ��Ϸ��ɫ
constexpr bool RED	= true;
constexpr bool BLACK = false;
// ���̶�λ 0x33-0xcb
constexpr uint8_t BOARD_LEFT = 3;
constexpr uint8_t BOARD_TOP = 3;
constexpr uint8_t BOARD_RIGHT = 11;
constexpr uint8_t BOARD_BOTTOM = 12;
// �������ӱ��
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
	void initGame();		// ��ʼ��
	void putChess(int32_t sq, int32_t pc, bool del = false);		// ��һ�����ӷ��������ϣ�delΪtrue�����Ƴ�����
public:
	bool cur_player = RED;	// ��ǰ��Ϸ��ɫ
	uint8_t	board[256];		// ��������
	uint8_t	pieces[48];		// ÿ�����ӵ�λ��
	int32_t	red_val;		// �����ֵ
	int32_t black_val;		// �����ֵ

	// ����
	Game();					// Ĭ�Ϲ��캯�� ���ڳ�ʼ��
	void changePlayer();	// �Ի���ɫ
	
	void buildFromFen(std::string fen);	// ����fen����������

	void takeOneMove(int32_t move);		// һ���ƶ�
	void deleteOneMove(int32_t move);	// ����һ���ƶ�

	// �ŷ�����
	void genAllMoves(Move moves[]);		// ���������ŷ�
	void genCapMoves(Move moves[]);		// ���ɳ����ŷ�
	int genNotCapMoves(Move moves[]);	// ���ɲ������ŷ�

	// ����
	void printForDebug();	// ����������ն�
};


// ����תΪ��������
inline int coordToPos(int row, int col)
{
	return col + (row << 4);
}

// ��ȡ���ӱ�ſ�ʼλ�� ����16 ����32
inline int sideTag(bool side)
{
	return side == RED ? 16 : 32;
}

// �����ƶ���Ϣ ��ȡ���
inline int getSrc(int32_t move)
{
	return move & 255;
}

// �����ƶ���Ϣ ��ȡ�յ�
inline int getDst(int32_t move)
{
	return move >> 8;
}

// ��ȡλ�õ�����Ϣ
inline int getIdxRow(int idx)
{
	return idx >> 4;
}

// ��ȡλ�õ�����Ϣ λ����ȡģ
inline int getIdxCol(int idx)
{
	return idx & 15; 
}

// ����յ���ϳ�һ��move����
inline int32_t getMoveType(int src, int dst)
{
	return src + (dst << 8);
}

// moveת�ַ���
inline std::string moveToString(int32_t move)
{
	std::string str;
	str += getIdxCol(getSrc(move)) - BOARD_LEFT + 'a';
	str += '9' - (getIdxRow(getSrc(move)) - BOARD_TOP);
	str += getIdxCol(getDst(move)) - BOARD_LEFT + 'a';
	str += '9' - (getIdxRow(getDst(move)) - BOARD_TOP);

	return str;
}

// �ַ���תmove
inline int32_t stringToMove(std::string str)
{
	int src = coordToPos('9' - str[1] + BOARD_TOP, str[0] - 'a' + BOARD_LEFT);
	int dst = coordToPos('9' - str[3] + BOARD_TOP, str[2] - 'a' + BOARD_LEFT);
	return getMoveType(src, dst);
}

// ��ȡ���Ӵ�д��ĸ��Ӧ������
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
