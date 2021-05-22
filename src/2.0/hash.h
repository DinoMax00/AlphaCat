#ifndef HASH
#define HASH

#include <stdint.h>

//÷√ªª±Ì±Í÷æ
const int FLAG_BETA = 1;
const int FLAG_ALPHA = 2;
const int FLAG_PV = 3;

struct HashItem
{
	uint64_t zobrist;
	uint16_t bestmove;
	uint8_t depthAlpha;
	uint8_t depthBeta;
	uint16_t valueAlpha;
	uint16_t valueBeta;
};

void recordHashTable(uint64_t zobr, int flag, int value, int depth, int step);
int getHashTable(uint64_t zobr, int vlAlpha, int vlBeta, int depth, int& step);


extern HashItem* HashTable;
extern int HashSize;

inline void newTable()
{
	HashTable = new HashItem[HashSize + 1];
}
inline void delTable()
{
	delete[] HashTable;
}

#endif