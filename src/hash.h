#ifndef HASHNUM_H
#define HASHNUM_H

#include"base.h"
#include<ctime>
#include<cstdlib>

class Hash
{
public:
	void initHash();
	long long getHash(unsigned char chess, unsigned char pos);
private:
	long long rand64();
	long long hash[14][256];
	bool loaded = false;
};

#endif
