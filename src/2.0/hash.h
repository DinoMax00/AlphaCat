#ifndef HASH
#define HASH

#include <stdint.h>
#include <string.h>
#include "game.h"
//÷√ªª±Ì±Í÷æ
const int FLAG_BETA = 1;
const int FLAG_ALPHA = 2;
const int FLAG_PV = 3;

struct HashItem
{
	uint64_t zobrist;
	uint16_t bestmove;
	int16_t flag;
	int16_t depth;
	int16_t value;
};

void recordHashTable(uint64_t zobr, int flag, int value, int depth, int step);
int getHashTable(Game& pos, int vlAlpha, int vlBeta, int depth, bool no_null, int& step);


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

inline void clearHashTable()
{
	memset(HashTable, 0, sizeof(HashItem) * (HashSize + 1));
}

#endif