#include "hash.h"
#include "game.h"
#include "search.h"

HashItem* HashTable;
int HashSize = 1024 * 1024 - 1;

void recordHashTable(uint64_t zobr, int flag, int value, int depth, int step)
{
	if (zobr == HashTable[zobr & HashSize].zobrist && (HashTable[zobr & HashSize].depthAlpha < depth || HashTable[zobr & HashSize].depthBeta < depth))
		return;
	HashItem hash;
	hash.zobrist = zobr;
	hash.bestmove = step;
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
int getHashTable(uint64_t zobr, int vlAlpha, int vlBeta, int depth, int& step)
{
	HashItem hash;
	hash = HashTable[zobr & HashSize];
	if (hash.zobrist != zobr)
		return -MATE_VALUE;
	if (hash.depthAlpha > 0)
	{
		int value = hash.valueAlpha;
		if (hash.depthAlpha >= depth && value <= vlAlpha)
			return value;
	}
	if (hash.depthBeta > 0)
	{
		int value = hash.valueBeta;
		if (hash.depthBeta >= depth && value >= vlBeta)
			return value;
	}
	return -MATE_VALUE;
}