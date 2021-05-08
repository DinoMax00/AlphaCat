#include <iostream>
#include"hash.h"



Hash::Hash()
{
	srand(time(0));
	TT = new TransTable[1024 * 1024];
	for (int i = 0; i < 1024 * 1024; i++)
		TT[i].dep = -1;
	return;
}

long long Hash::getHash(unsigned char chess, unsigned char pos)
{
	if (0 < chess && chess < 8)
		return zor_hash[convert_map[chess]-7][pos - 1];
	else if (100 < chess && chess < 108)
		return zor_hash[convert_map[chess - 100]][pos - 1];
	else return 0;
}

int Hash::searchFromTransTable(long long Zobrist, int dep, player_type player, int alpha, int beta)
{
	int num = (int)(Zobrist & (long long)0xFFFFF);
	if (TT[num].Zobrist == Zobrist)
	{
		if (TT[num].dep >= dep)
		{
			if (TT[num].flag == hashExact)
				return TT[num].value;
			if (TT[num].flag == hashAlpha && TT[num].value <= alpha)
				return alpha;
			if (TT[num].flag == hashBeta && TT[num].value >= beta)
				return beta;
		}
	}
	return -10000000;
}

void Hash::saveInTransTable(long long Zobrist, int dep, player_type player, int flag, int value)
{
	int num = (int)(Zobrist & (long long)0xFFFFF);
	if (TT[num].dep > dep)
		return;
	TT[num].Zobrist = Zobrist;
	TT[num].dep = dep;
	TT[num].flag = flag;
	TT[num].value = value;
}