#include <iostream>

#include "hash.h"
#include "game.h"
#include "search.h"

HashItem* HashTable;
int HashSize = 1024 * 1024 - 1;

void recordHashTable(uint64_t zobr, int flag, int value, int depth, int step)
{
	if (zobr == HashTable[zobr & HashSize].zobrist && (HashTable[zobr & HashSize].depthAlpha >= depth || HashTable[zobr & HashSize].depthBeta >= depth))
		return;
	HashItem hash;
	hash.zobrist = zobr;
	hash.bestmove = step;
	hash.depthAlpha = 0;
	hash.depthBeta = 0;
	if (flag & FLAG_ALPHA)
	{
		hash.depthAlpha = depth;
		hash.valueAlpha = value;
	}
	if (flag & FLAG_BETA)
	{
		hash.depthBeta = depth;
		hash.valueBeta = value;
	}
	HashTable[zobr & HashSize] = hash;
}

int getHashTable(uint64_t zobr, int vlAlpha, int vlBeta, int depth, bool no_null, int& step)
{
	HashItem hash;
	hash = HashTable[zobr & HashSize];
	if (hash.zobrist != zobr)
		return -MATE_VALUE;
	step = hash.bestmove;
	if (hash.depthBeta > 0)
	{
		int value = hash.valueBeta;
		if (!(no_null && hash.bestmove == 0) && hash.depthBeta >= depth && value >= vlBeta)
			return value;
	}
	if (hash.depthAlpha > 0)
	{
		int value = hash.valueAlpha;
		if (hash.depthAlpha >= depth && value <= vlAlpha)
			return value;
	}
	return -MATE_VALUE;
}

