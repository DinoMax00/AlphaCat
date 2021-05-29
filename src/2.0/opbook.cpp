#include <iostream>
#include <stdint.h>
#include "bookrecord.h"

uint16_t moveMirror(uint16_t move)
{
	int from = move / 256;
	int to = move % 256;
	from = from + 14 - 2 * (from % 16);
	to = to + 14 - 2 * (to % 16);
	uint16_t mirror = from * 256 + to;
	return mirror;
}
uint16_t opBookSearch(uint64_t obj, int mirror)
{
	//std::ofstream temp;
	//temp.open("./oplog.txt", std::ios::out| std::ios::app);
	int bestmove = 0;
	int16_t score = -100;
	int length = 156300;
	int left = 0, right = length - 1, mid = (left + right) / 2;
	int i = 0;
	//temp << obj << std::endl;
	while (left <= right)
	{
		uint64_t now = opbook_zobrist[mid];
		if (obj == now)
			break;
		else if (obj < now)
			right = mid - 1;
		else if (obj > now)
			left = mid + 1;
		mid = (left + right) / 2;
	}
	if (left > right)
		return 0;
	while (mid >= 1 && obj == opbook_zobrist[mid - 1])
	{
		mid--;
	}
	while (mid < length && obj == opbook_zobrist[mid])
	{
		int16_t now_score = opbook_score[mid];
		uint64_t now = opbook_zobrist[mid];
		if (now_score > score)
		{
			score = now_score;
			bestmove = opbook_move[mid];
		}
		mid++;
	}
	int from = bestmove % 256;
	int to = bestmove / 256;
	bestmove = from * 256 + to;
	if (mirror)
		bestmove = moveMirror(bestmove);
	return bestmove;
}