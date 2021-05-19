/*
#ifndef SEARCH_H
#define SEARCH_H

#include "game.h"
#include "pregen.h"

const int LIMIT_DEPTH = 64;             // 搜索的极限深度
const int MAX_MOVE_NUM = 1024;          // 局面能容纳的回滚着法数
const int MATE_VALUE = 10000;           // 最高分值，即将死的分值
const int WIN_VALUE = MATE_VALUE - 200; // 搜索出胜负的分值界限，超出此值就说明已经搜索出杀棋了

// 搜索前可设置的全局变量，指定搜索参数
struct SearchStruct {
    Game pos;                   // 有待搜索的局面
    bool bQuit, bPonder, bDraw; // 是否收到退出指令、后台思考模式和提和模式
    // bool bBatch, bDebug;        // 是否批处理模式和调试模式
    // bool bUseHash, bUseBook;    // 是否使用置换表裁剪和开局库
    // bool bNullMove, bKnowledge; // 是否空着裁剪和使用局面评价知识
    bool bIdle;                 // 是否空闲
    // RC4Struct rc4Random;               // 随机数,暂不需要
    int nGoMode, nNodes, nCountMask; // 搜索模式、结点数和
    int nProperTimer, nMaxTimer;     // 计划使用时间
    //   int nRandomMask, nBanMoves;        // 随机性屏蔽位和禁着数
    //   uint16_t wmvBanList[MAX_MOVE_NUM]; // 禁着列表
    char szBookFile[1024]; // 开局库
    int mvResult;          // 返回着法
};
// 搜索的启动过程
uint16_t SearchMain(int nDepth);

inline int64_t GetTime() {
  timeb tb;
  ftime(&tb);
  return (int64_t) tb.time * 1000 + tb.millitm;
}

extern SearchStruct Search;

#endif
*/