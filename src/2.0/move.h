#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>

struct Move
{
	uint8_t src;	// ���
	uint8_t	dst;	// �յ�
	int8_t	captureed; // ������
};


#endif
