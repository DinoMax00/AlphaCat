#include"hash.h"

long long Hash::rand64()
{
	long long ans = rand() ^ ((long long)rand() << 15) ^ ((long long)rand() << 30) ^ ((long long)rand() << 45) ^ ((long long)rand() << 60);
	return ans;
}

void Hash::initHash()
{
	srand(time(0));
	for (int i = 0x30; i <= 0xc0; i += 0x10)
		for (int j = 0x04; j <= 0x0c; j++)
			for (int k = 0; k < 14; k++)
				hash[k][i + j] = rand64();
	loaded = true;
	return;
}

long long Hash::getHash(unsigned char chess, unsigned char pos)
{
	if (chess == EMPTY)
		return 0;
	if (chess < 100)
		return hash[chess - 1][pos];
	else
		return hash[chess - 94][pos];
}