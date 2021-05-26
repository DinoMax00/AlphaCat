#ifndef SEARCH_H
#define SEARCH_H

#include "game.h"
#include "pregen.h"

const int LIMIT_DEPTH = 64;             // �����ļ������
const int MATE_VALUE = 10000;           // ��߷�ֵ���������ķ�ֵ
const int BAN_VALUE = MATE_VALUE - 100; // �����и���ֵ
const int WIN_VALUE = MATE_VALUE - 200; // ������ʤ���ķ�ֵ���ޣ�������ֵ��˵���Ѿ�������ɱ����
const int NULL_DEPTH = 2;				// ���ż��õ����
// ����ǰ�����õ�ȫ�ֱ�����ָ����������
struct SearchStruct {
	Game pos;                   // �д������ľ���
	int result = -1;			// �����ŷ�

	bool stop;					// �Ƿ�ʱ
	uint64_t cur_time;			// ��ʱ��	
	uint64_t time_limit = 2000;		// ��ʱ
};

// ��������������
void searchMain(int depth);

extern SearchStruct Search;

#endif