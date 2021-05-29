#ifndef SEARCH_H
#define SEARCH_H

#include <string.h>

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
		memset(killeTable, 0, sizeof(killeTable));
	}
};

// ��������������
void searchMain(int depth);

extern SearchStruct Search;

#endif