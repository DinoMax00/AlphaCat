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
constexpr bool RED = true;
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

// �������ӵ�����λ
constexpr uint32_t JIANG_BITPIECE = 1 << JIANG_FROM;
constexpr uint32_t SHI_BITPIECE = (1 << SHI_FROM) | (1 << SHI_TO);
constexpr uint32_t XIANG_BITPIECE = (1 << XIANG_FROM) | (1 << XIANG_TO);
constexpr uint32_t MA_BITPIECE = (1 << MA_FROM) | (1 << MA_TO);
constexpr uint32_t JU_BITPIECE = (1 << JU_FROM) | (1 << JU_TO);
constexpr uint32_t PAO_BITPIECE = (1 << PAO_FROM) | (1 << PAO_TO);
constexpr uint32_t BING_BITPIECE = (1 << BING_FROM) | (1 << (BING_FROM + 1)) |
(1 << (BING_FROM + 2)) | (1 << (BING_FROM + 3)) | (1 << BING_TO);
constexpr uint32_t ATTACK_BITPIECE = MA_BITPIECE | JU_BITPIECE | PAO_BITPIECE | BING_BITPIECE;

/* ������Ŷ�Ӧ����������
 *
 * ������Ŵ�0��47������0��15���ã�16��31��ʾ���ӣ�32��47��ʾ���ӡ�
 * ÿ��������˳�������ǣ�˧�������������������ڱ���������(��ʿʿ��������������������������)
 * ��ʾ���ж������Ǻ�����"pc < 32"��������"pc >= 32"
 */
const int pieceType[48] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
};

class Game
{
private:
	void initGame();		// ��ʼ��
	void putChess(int32_t sq, int32_t pc, bool del = false);		// ��һ�����ӷ��������ϣ�delΪtrue�����Ƴ�����
public:
	bool cur_player = RED;	// ��ǰ��Ϸ��ɫ
	int32_t	red_val;		// �����ֵ
	int32_t black_val;		// �����ֵ
	union {
		uint32_t bitPieces;		// 16-32�������Ƿ��������� ????16-47��
		uint16_t splited_bitPieces[2];// 0Ϊ��ɫ�������ӣ�1Ϊ��ɫ��������
	};


	uint8_t	board[256];		// ��������
	uint8_t	pieces[48];		// ÿ�����ӵ�λ��
	uint16_t bitRow[16];	// λ��
	uint16_t bitCol[16];	// λ��



	// ����
	Game();					// Ĭ�Ϲ��캯�� ���ڳ�ʼ��
	void changePlayer();	// �Ի���ɫ

	void buildFromFen(std::string fen);	// ����fen����������

	void takeOneMove(int32_t move);		// һ���ƶ�
	void deleteOneMove(int32_t move);	// ����һ���ƶ�

	// �ŷ�����
	int genAllMoves(Move moves[]);		// ���������ŷ�
	int genCapMoves(Move moves[]);		// ���ɳ����ŷ�
	int genNonCapMoves(Move moves[], int start = 0);	// ���ɲ������ŷ�

	//�������
	bool detectCheck();

	// ����
	void printForDebug();	// ����������ն�

	// ����������������
	void evaBoard();
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

// ˫��������
inline uint32_t bothBitpiece(uint32_t bitPiece) {
	return bitPiece + (bitPiece << 16);
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