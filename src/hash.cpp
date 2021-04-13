#include"hash.h"

long long Hash::rand64()
{
	long long ans = rand() ^ ((long long)rand() << 15) ^ ((long long)rand() << 30) ^ ((long long)rand() << 45) ^ ((long long)rand() << 60);
	return ans;
}

Hash::Hash()
{
	srand(time(0));
	for (int i = 0x30; i <= 0xc0; i += 0x10)
		for (int j = 0x04; j <= 0x0c; j++)
			for (int k = 0; k < 14; k++)
				hash[k][i + j] = rand64();
	TT[0] = new TransTable[1024 * 1024];
	TT[1] = new TransTable[1024 * 1024];
	for (int i = 0; i < 1024 * 1024; i++)
		TT[0][i].dep = TT[1][i].dep = -1;
	return;
}

long long Hash::getHash(unsigned char chess, unsigned char pos)
{
	if (0 < chess && chess < 8)
		return hash[chess - 1][pos];
	else if (100 < chess && chess < 108)
		return hash[chess - 94][pos];
	else return 0;
}

int Hash::searchFromTransTable(long long Zobrist, int dep, player_type player, int alpha, int beta)
{
	int num = (int)(Zobrist & (long long)0xFFFFF);
	if (TT[player][num].Zobrist == Zobrist)
	{
		if (TT[player][num].dep >= dep)
		{
			if (TT[player][num].flag == 0)
				return TT[player][num].value;
			if (TT[player][num].flag == 1 && TT[player][num].value <= alpha)
				return alpha;
			if (TT[player][num].flag == 2 && TT[player][num].value >= beta)
				return beta;
		}	
	}
	return -10000000;
}

void Hash::saveInTransTable(long long Zobrist, int dep, player_type player, int flag, int value)
{
	int num = (int)(Zobrist & (long long)0xFFFFF);
	if (TT[player][num].dep > dep)
		return;
	TT[player][num].Zobrist = Zobrist;
	TT[player][num].dep = dep;
	TT[player][num].flag = flag;
	TT[player][num].value = value;
}