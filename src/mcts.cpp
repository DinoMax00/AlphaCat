#include"mcts.h"
#include<math.h>
#include"log.h"
mcts::mcts(Board* bo)
{
	situation = bo;
	initial_mcts = this;
	all_times = 1;
	//此处调用快速获胜获得结果
	win_times = 0;
	if (!bo->mov.empty())
		is_over = 0;
	else
		is_over = 1;
	tryed_choices.reserve(MAX_MOVES);
	father = this;
}


void mcts::selectionOfTry()
{
	int i = 0;
	while (i++ < mcts_TIMES) 
	{
		//printFormctsDebug();
		//std::cout << situation->player << std::endl;
		mcts* selected = this;
		//std::cout << ">>fuck" << std::endl;
		if (selected->situation->mov.empty() || selected->is_over) //Log().info("error by feifei");
			return;
		while (selected->tryed_choices.size() == selected->situation->mov.size())
		{
			//std::cout << ">>fuck" << std::endl;
			double max_score = -1;
			mcts* next = selected;
			for (auto i : selected->tryed_choices)
			{
				if (i->situation->mov.empty() || i->is_over) continue;
				double score = -10000;
				if (selected->situation->player == initial_mcts->situation->player)
					score = (double)i->win_times / i->all_times + mcts_C * sqrt(log(selected->all_times) / i->all_times);
				else
					score = ((double)i->all_times - i->win_times) / i->all_times + mcts_C * sqrt(log(selected->all_times) / i->all_times);
				if (score > max_score)
				{
					max_score = score;
					next = i;
				}
			}
			//std::cout << ">>fuck" << std::endl;
			if (next == selected)
			{
				selected->is_over = true;
				selected = selected->father;
			}
			else
				selected = next;
			if (selected == initial_mcts)
				break;
		}
		if (selected == initial_mcts && selected->tryed_choices.size() == selected->situation->mov.size())
		{
			Log().info("error by feifei2");
			std::cout << "??" << std::endl;
			return;
		}
		Move& to_new_situation = selected->situation->mov[selected->tryed_choices.size()];
		Board* next_board = new Board(selected->situation, to_new_situation);
		
		if(next_board->mov.empty())
		{
			mcts* next_mcts = new mcts(next_board);
			int win_time;
			if(selected->situation->player==initial_mcts->situation->player)
				win_time=100;
			else win_time=0;
			next_mcts->win_times=win_time;
			next_mcts->all_times=100;
			selected->is_over = true;
			next_mcts->initial_mcts = initial_mcts;
			next_mcts->father = selected;
			// selected->tryed_choices.clear();
			selected->tryed_choices.emplace_back(next_mcts);
			while(selected!=initial_mcts){
				selected=selected->father;
				selected->all_times+=100;
				selected->win_times+=100;
			}
			continue;
		}

		Board* board_play = new Board(selected->situation, to_new_situation);
		int result = board_play->mctsMove();
		

		mcts* next_mcts = new mcts(next_board);
		int x;
		int times = 1;
		// if(next_board->mov.empty()) times=1;
		// if(result>0) {
		// 	int cnt =0;
		// 	while(result>50 && cnt++<20){
		// 		board_play = new Board(selected->situation, to_new_situation);
		// 		result = board_play->mctsMove();
		// 	}
		// }
		//unsigned int result = situation->mctsMove();
		if (result <= 0 && next_board->player == initial_mcts->situation->player || result >= 0 && next_board->player != initial_mcts->situation->player)
			x=next_mcts->win_times = 0;
		else
			x=next_mcts->win_times = 1*times;
		// {
		// 	std::cout<<"player::"<<next_board->player<<std::endl;
		// 	next_board->printBoardForDebug2();
		// 	std::cout<<"result::"<<result<<"   X: "<<x<<std::endl;
		// 	std::cout<<std::endl;
		// 	std::cout<<std::endl;
		// }
		next_mcts->initial_mcts = initial_mcts;
		next_mcts->father = selected;
		selected->tryed_choices.emplace_back(next_mcts);

		next_mcts->win_times;
		
		while (selected != initial_mcts) 
		{
			 selected->all_times += 1*times;
			 selected->win_times += x;
			 selected = selected->father;
		}

		selected->all_times += 1*times;
		selected->win_times += x;
	}
	// printFormctsDebug();
	printFormctsDebug2();
}


Move& mcts::getBestMove()
{
	double max_win_score = -1;
	if (tryed_choices.empty())
	{
		Move* x = new Move("a0i9");
		Log().info("error by feifei3");
		return *x;
	}
	int choice = 0;
	int cnt = 0;
	for (auto i : tryed_choices)
	{
		double win_score = (double)i->win_times / i->all_times;
		//std::cout << situation->mov[cnt].moveToString() << std::endl;
		//std::cout << win_score << std::endl;
		if (win_score > max_win_score)
		{
			max_win_score = win_score;
			choice = cnt;
		}
		cnt++;
	}
	return situation->mov[choice];
}

std::string mcts::getBestMoveString()
{
	double max_win_score = -1;
	if (tryed_choices.empty())
	{
		Move x("a0i9");
		Log().info("error by feifei3");
		return x.moveToString();
	}
	int choice = 0;
	int cnt = 0;
	for (auto i : tryed_choices)
	{
		double win_score = (double)i->win_times/i->all_times;
		unsigned int wintime =0;
		//std::cout << situation->mov[cnt].moveToString() << std::endl;
		//std::cout << i->win_times<<' '<<i->all_times << std::endl;
		//std::cout << win_score << std::endl;
		//std::cout << std::endl;
		if (win_score > max_win_score||win_score==max_win_score && i->all_times>wintime)
		{
			max_win_score = win_score;
			wintime = i->all_times;
			choice = cnt;
		}
		cnt++;
	}
	return situation->mov[choice].moveToString();
}

void mcts::printFormctsDebug() 
{
	std::cout<<"all results"<<std::endl;
	std::cout << win_times << " " << all_times << std::endl;
	mcts* now=this;
	int t=0;
	while(t<tryed_choices.size())
	{
		std::cout<<"tryed_choices:"<<now->situation->mov[t].moveToString()<<std::endl;
		std::cout << now->tryed_choices[t]->win_times << " " << now->tryed_choices[t]->all_times << std::endl;
		t++;
		std::cout<<std::endl;
	}
	// std::cout << tryed_choices.size()<<" "<<this->situation->mov.size()<<std::endl;
	// for (int i = 0; i < (int)tryed_choices.size(); i++) 
	// {
	// 	std::cout << situation->mov[i].moveToString() << std::endl;
	// 	std::cout << tryed_choices[i]->win_times << " " << tryed_choices[i]->all_times << std::endl;
	// }
	//this->situation->printBoardForDebug2();
	std::cout << std::endl;
	/*for (auto i : tryed_choices) 
	{
		std::cout << i->win_times << " " << i->all_times << std::endl;
		std::cout << i->tryed_choices.size() << std::endl;
		std::cout << std::endl;
	}*/
}
void mcts::printFormctsDebug2() 
{
	// std::cout<<"all results"<<std::endl;
	Log().info("allresult::"+std::to_string(win_times)+' '+std::to_string(all_times));
	// std::cout << win_times << " " << all_times << std::endl;
	mcts* now=this;
	int t=0;
	while(t<tryed_choices.size())
	{
		// std::cout<<"tryed_choices:"<<now->situation->mov[t].moveToString()<<std::endl;
		Log().add("tryed_choices:"+now->situation->mov[t].moveToString());
		// std::cout << now->tryed_choices[t]->win_times << " " << now->tryed_choices[t]->all_times << std::endl;
		Log().add("result::" + std::to_string(now->tryed_choices[t]->win_times) + " " + std::to_string(now->tryed_choices[t]->all_times));
		t++;
		// std::cout<<std::endl;
	}
	
}