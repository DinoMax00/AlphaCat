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
	int searchAlphaBeta(Board&, int depth, int alpha, int beta, int top);
	int best;
	Move moveInDep[MAX_SEARCH_DEPTH + 1][MAX_MOVES];
};

#endif