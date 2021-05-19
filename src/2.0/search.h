/*
#ifndef SEARCH_H
#define SEARCH_H

#include "game.h"
#include "pregen.h"

const int LIMIT_DEPTH = 64;             // �����ļ������
const int MAX_MOVE_NUM = 1024;          // ���������ɵĻع��ŷ���
const int MATE_VALUE = 10000;           // ��߷�ֵ���������ķ�ֵ
const int WIN_VALUE = MATE_VALUE - 200; // ������ʤ���ķ�ֵ���ޣ�������ֵ��˵���Ѿ�������ɱ����

// ����ǰ�����õ�ȫ�ֱ�����ָ����������
struct SearchStruct {
    Game pos;                   // �д������ľ���
    bool bQuit, bPonder, bDraw; // �Ƿ��յ��˳�ָ���̨˼��ģʽ�����ģʽ
    // bool bBatch, bDebug;        // �Ƿ�������ģʽ�͵���ģʽ
    // bool bUseHash, bUseBook;    // �Ƿ�ʹ���û���ü��Ϳ��ֿ�
    // bool bNullMove, bKnowledge; // �Ƿ���Ųü���ʹ�þ�������֪ʶ
    bool bIdle;                 // �Ƿ����
    // RC4Struct rc4Random;               // �����,�ݲ���Ҫ
    int nGoMode, nNodes, nCountMask; // ����ģʽ���������
    int nProperTimer, nMaxTimer;     // �ƻ�ʹ��ʱ��
    //   int nRandomMask, nBanMoves;        // ���������λ�ͽ�����
    //   uint16_t wmvBanList[MAX_MOVE_NUM]; // �����б�
    char szBookFile[1024]; // ���ֿ�
    int mvResult;          // �����ŷ�
};
// ��������������
uint16_t SearchMain(int nDepth);

inline int64_t GetTime() {
  timeb tb;
  ftime(&tb);
  return (int64_t) tb.time * 1000 + tb.millitm;
}

extern SearchStruct Search;

#endif
*/