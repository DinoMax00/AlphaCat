#include "hash.h"
#include "game.h"
#include "search.h"

HashItem* HashTable;
int HashSize = 1024 * 1024 * 4 - 1;


void recordHashTable(uint64_t zobr, int flag, int value, int depth, int step)
{
	if (zobr == HashTable[zobr & HashSize].zobrist && HashTable[zobr & HashSize].depth >= depth)
		return;
	if (flag == FLAG_BETA && HashTable[zobr & HashSize].bestmove != 0 && step == 0)
		return;
	HashItem hash;
	hash.zobrist = zobr;
	hash.bestmove = step;
	hash.flag = flag;
	hash.depth = depth;
	hash.value = value;
	HashTable[zobr & HashSize] = hash;
}

int getHashTable(Game& pos, int vlAlpha, int vlBeta, int depth, bool no_null, int& step)
{
	HashItem hash = HashTable[pos.zobrist & HashSize];
	if (hash.zobrist != pos.zobrist)
		return -MATE_VALUE;
	step = hash.bestmove;
	int value = hash.value;
	bool mate = false;

	//杀棋步数调整
	if (value > WIN_VALUE)
	{
		mate = true;
		if (value > BAN_VALUE)
			value -= pos.depth;
		else
			return -MATE_VALUE;
	}
	else if (value < -WIN_VALUE)
	{
		mate = true;
		if (value < -BAN_VALUE)
			value += pos.depth;
		else
			return -MATE_VALUE;
	}

	if (hash.depth >= depth || mate)
	{
		if (hash.flag == FLAG_PV)
		{
			return value;
		}
		else if (hash.flag == FLAG_BETA)
		{
			if (!(no_null && hash.bestmove == 0) && value >= vlBeta)
				return value;
		}
		else if (hash.flag == FLAG_ALPHA)
		{
			if (value <= vlAlpha)
				return value;
		}
	}
	return -MATE_VALUE;
}
