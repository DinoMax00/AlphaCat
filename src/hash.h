#ifndef HASHNUM_H
#define HASHNUM_H

#include"base.h"
#include<ctime>
#include<cstdlib>

struct TransTable
{
	long long Zobrist;
	int dep;
	int flag;
	int value;
};

class Hash
{
public:
	Hash();
	long long getHash(unsigned char chess, unsigned char pos);
	int searchFromTransTable(long long Zobrist, int dep, player_type player, int alpha, int beta);
	void saveInTransTable(long long Zobrist, int dep, player_type player, int flag, int value);
private:
	long long rand64();
	long long hash[14][256];
	TransTable* TT[2];
};

#endif
