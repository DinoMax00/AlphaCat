#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>

struct Move
{
	int32_t step;	// move����
	uint8_t src;	// ���
	uint8_t	dst;	// �յ�
	uint8_t	captureed; // ������
	uint8_t chk;	   // �ŷ��Ƿ񽫾�
};
union CapMove
{
	Move mov;
	int val;
};


#endif
