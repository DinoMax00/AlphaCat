#ifndef SEARCH_H
#define SEARCH_H
#include"board.h"
#include"move.h"
#include"base.h"


const int inf = int(1e9);

class searcher
{
public:
	Move getBestMove(Board&);
private:
	int searchAlphaBeta(Board&, int depth, int alpha, int beta);
	int best;
	int start, end;
	int max_deep;
	Move moveInDep[8][MAX_MOVES];
};

#endif