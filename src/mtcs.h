#ifndef MTCS_H
#define MTCS_H
#include<string>
#include<vector>
#include"move.h"
#include"board.h"

const double MTCS_C = 1.414;
const unsigned int MTCS_TIMES = 10;

class Mtcs {
	Board* situation;
	unsigned int win_times;
	unsigned int all_times;
	bool is_over;
	Mtcs* initial_mtcs;
	//Move& best_move;
	Mtcs* father;
	//unsigned int draw_times;

	std::vector<Mtcs*> tryed_choices;

	Mtcs(Board*);
	
	void selectionOfTry();

	Move& getBestMove();

	void printForMtcsDebug();

	//void ExpansionOfNew();
};


#endif