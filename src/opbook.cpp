#include <fstream>
#include "opbook.h"

opBook::opBook()
{
	infile.open("./opbook.txt", std::ios::in);
}

int opBook::opBook_search(unsigned long long obj)
{
	//std::ofstream temp;
	//temp.open("./oplog.txt", std::ios::out| std::ios::app);
	int bestmove = 0;
	int score = 0;
	int length = 139999;
	int left = 0, right = length - 1, mid = (left + right) / 2;
	int i = 0;
	//temp << obj << std::endl;
	while (left<=right)
	{
		infile.seekg(48 * mid, std::ios::beg);
		unsigned long long now;
		infile >> now;
		if (obj == now)
			break;
		else if (obj < now)
			right = mid-1;
		else if (obj > now)
			left = mid+1;
		mid = (left + right) / 2;
	}
	if (left > right)
		return -1;
	while (1)
	{
		unsigned long long now;
		infile.seekg(48 * mid, std::ios::beg);
		infile >> now;
		if (obj != now)
			break;
		mid--;
	}
	mid++;
	while (1)
	{
		int now_move, now_score;
		unsigned long long now;
		infile.seekg(48 * mid, std::ios::beg);
		infile >> now >> now_move >> now_score;
		if (obj != now)
			break;
		if (now_score > score)
		{
			score = now_score;
			bestmove = now_move;
		}
		mid++;
	}
	return bestmove;
}