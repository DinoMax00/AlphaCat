#ifndef SEARCH_H
#define SEARCH_H

#include "game.h"
#include "pregen.h"

const int LIMIT_DEPTH = 64;             // �����ļ������
const int MATE_VALUE = 10000;           // ��߷�ֵ���������ķ�ֵ
const int WIN_VALUE = MATE_VALUE - 200; // ������ʤ���ķ�ֵ���ޣ�������ֵ��˵���Ѿ�������ɱ����

// ����ǰ�����õ�ȫ�ֱ�����ָ����������
struct SearchStruct {
	Game pos;                   // �д������ľ���
	int result;					// �����ŷ�
};

// ��������������
void searchMain(int depth);

extern SearchStruct Search;

#endif