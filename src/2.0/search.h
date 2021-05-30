#ifndef SEARCH_H
#define SEARCH_H

#include <string.h>
#include <iostream>
#include "game.h"
#include "pregen.h"

struct SearchStruct {
	Game pos;                   // �д������ľ���
	int result = -1;			// �����ŷ�
	int nodes = 0;				// �����ڵ�����

	bool stop;					// �Ƿ�ʱ
	uint64_t cur_time = 0;			// ��ʱ��	
	uint64_t time_limit = 2000;		// ��ʱ

	uint16_t killeTable[LIMIT_DEPTH][2] = {0}; // ɱ���ŷ�

	void clearKiller()
	{
		for (int i = 0; i < LIMIT_DEPTH; i++) killeTable[i][0] = killeTable[i][1] = 0;
	}
};

// ��������������
void searchMain(int depth);

extern SearchStruct Search;

#endif