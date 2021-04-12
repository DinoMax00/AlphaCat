#ifndef HASHNUM_H
#define HASHNUM_H

#include"base.h"
#include<ctime>
#include<cstdlib>

class Hash
{
public:
	Hash();
	long long getHash(unsigned char chess, unsigned char pos);
private:
	long long rand64();
	long long hash[14][256];
};

class TransTable
{
public:
	long long Zobrist;
	int max_dep;
	int dep;
	int flag;
	int value;
};
TransTable* Table[2];

#endif
