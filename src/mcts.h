#ifndef mcts_H
#define mcts_H
#include<string>
#include<vector>
#include"move.h"
#include"board.h"

const double mcts_C = 0.5;
const unsigned int mcts_TIMES = 10;

class mcts {
public:
	Board* situation;
	unsigned int win_times;
	unsigned int all_times;
	double p_of_win;
	double point_all;
	bool is_over;
	mcts* initial_mcts;
	//Move& best_move;
	mcts* father;
	mcts* best_move_after;
	Move from_move;
	//unsigned int draw_times;

	std::vector<mcts*> tryed_choices;

	mcts(Board*);
	
	void selectionOfTry();

	// Move& getBestMove();
	std::string getBestMoveString();
	void printFormctsDebug();
	void printFormctsDebug2();
	mcts* new_world(Board*);
	//void ExpansionOfNew();
};


#endif