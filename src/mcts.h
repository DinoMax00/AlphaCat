#ifndef mcts_H
#define mcts_H
#include<string>
#include<vector>
#include"move.h"
#include"board.h"

const double mcts_C = 0.5;
const unsigned int mcts_TIMES = 10000;

class mcts {
public:
	Board* situation;
	unsigned int win_times;
	unsigned int all_times;
	bool is_over;
	mcts* initial_mcts;
	//Move& best_move;
	mcts* father;
	//unsigned int draw_times;

	std::vector<mcts*> tryed_choices;

	mcts(Board*);
	
	void selectionOfTry();

	Move& getBestMove();
	std::string getBestMoveString();
	void printFormctsDebug();
	void printFormctsDebug2();

	//void ExpansionOfNew();
};


#endif