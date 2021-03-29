#include "mcts.h"
#include <math.h>
#include "log.h"
mcts::mcts(Board *bo)
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
	// if(bo->blackValue<0) bo->blackValue=0;
	// if(bo->redValue<0) bo->redValue=0;
	p_of_win=(double)bo->redValue/(bo->redValue+bo->blackValue);
	point_all=p_of_win;
	tryed_choices.reserve(MAX_MOVES);
	father = this;
	best_move_after = this;
}

void mcts::selectionOfTry()
{
	int i = 0;
	while (i++ < mcts_TIMES)
	{
		//printFormctsDebug();
		//std::cout << situation->player << std::endl;
		mcts *selected = this;
		//std::cout << ">>fuck" << std::endl;
		if (selected->situation->mov.empty() || selected->is_over) //Log().info("error by feifei");
			break;
		while (selected->tryed_choices.size() == selected->situation->mov.size() && !selected->is_over)
		{
			//std::cout << ">>fuck" << std::endl;
			double max_score = -1;
			mcts *next = selected;
			for (auto i : selected->tryed_choices)
			{
				// if (i->is_over)
				// {
				// 	if (selected->situation->player == initial_mcts->situation->player && i->win_times == i->all_times)
				// 	{
				// 		selected->is_over = true;
				// 		mcts *t = selected;
				// 		// selected->tryed_choices.clear();
				// 		selected->tryed_choices.emplace_back(i);
				// 		selected->best_move_after=i;
				// 		int win_time = 100;
				// 		t->all_times += win_time;
				// 		t->win_times = t->all_times;
				// 		while (t != initial_mcts)
				// 		{
				// 			t = t->father;
				// 			t->all_times += win_time;
				// 			t->win_times += win_time;
				// 		}
				// 	}
				// 	else if (selected->situation->player != initial_mcts->situation->player && i->win_times == 0)
				// 	{
				// 		selected->is_over = true;
				// 		mcts *t = selected;
				// 		// selected->tryed_choices.clear();
				// 		selected->tryed_choices.emplace_back(i);
				// 		selected->best_move_after=i;
				// 		int win_time = 100;
				// 		t->win_times = 0;
				// 		t->all_times += win_time;
				// 		while (t != initial_mcts)
				// 		{
				// 			t = t->father;
				// 			t->all_times += win_time;
				// 		}
				// 	}
				// 	continue;
				// }
				// if (i->situation->mov.empty() || i->is_over)
				// 	continue;
				double score = -10000;
				double p0 = i->point_all/i->all_times;
				if(initial_mcts->situation->player==BLACK)
					p0 = 1-p0; 
				if (selected->situation->player == initial_mcts->situation->player)
					score = (double)0.3*i->win_times / i->all_times + p0 + mcts_C * sqrt(log(selected->all_times) / i->all_times);
				else
					score = (0.3*(double)i->all_times - i->win_times) / i->all_times +(1-p0)+ mcts_C * sqrt(log(selected->all_times) / i->all_times);
				
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
				continue;
			}
			else
				selected = next;
			if (selected == initial_mcts)
				break;
		}
		// if (selected == initial_mcts && selected->tryed_choices.size() == selected->situation->mov.size())
		// {
		// 	// Log().info("error by feifei2");
		// 	// std::cout << "??" << std::endl;
		// 	break;
		// }
		if(initial_mcts->is_over||selected->is_over) break;
		Move to_new_situation = selected->situation->mov[selected->tryed_choices.size()];
		Board *next_board = new Board(selected->situation, to_new_situation);
		mcts *next_mcts = new mcts(next_board);
		next_mcts->from_move.copyOneMove(to_new_situation);

		// if (next_board->mov.empty())
		// {
		// 	// next_board->printBoardForDebug();
		// 	// selected->situation->printBoardForDebug();
		// 	int win_time;
		// 	if (selected->situation->player == initial_mcts->situation->player)
		// 		win_time = 100;
		// 	else
		// 		win_time = 0;
		// 	next_mcts->win_times = win_time;
		// 	next_mcts->all_times = 100;
		// 	next_mcts->initial_mcts = initial_mcts;
		// 	next_mcts->father = selected;

		// 	selected->all_times+=100;
		// 	selected->win_times+=win_time;
		// 	selected->is_over = true;

		// 	// selected->tryed_choices.clear();
		// 	selected->best_move_after=next_mcts;
		// 	selected->tryed_choices.emplace_back(next_mcts);

		// 	while (selected != initial_mcts)
		// 	{
		// 		selected = selected->father;
		// 		selected->all_times += 100;
		// 		selected->win_times += win_time;
		// 	}
		// 	continue;
		// }

		Board *board_play = new Board(selected->situation, to_new_situation);
		int result = board_play->mctsMove();
		// int result=1000;
		// {
		// 	int cnt = 0;
		// 	for (int i = 0; i < 3; i++)
		// 	{
		// 		Board *board_play2 = new Board(selected->situation, to_new_situation);
		// 		// Log().info("fashenshenmeshile");
		// 		int tmp = board_play2->mctsMove();
		// 		if (tmp > 0)
		// 		{
		// 			result = std::min(tmp, result);
		// 		}
		// 		else
		// 			cnt++;
		// 	}
		// 	if (cnt > 1 || result > 20)
		// 		result = -1;
		// 	else
		// 		result = 1;
		// }

		// mcts *next_mcts = new mcts(next_board);

		// next_mcts->from_move.copyOneMove(to_new_situation);

		int actual_result;
		if (result <= 0 && next_board->player == initial_mcts->situation->player || result >= 0 && next_board->player != initial_mcts->situation->player)
			actual_result = next_mcts->win_times = 0;
		else
			actual_result = next_mcts->win_times = 1;

		next_mcts->initial_mcts = initial_mcts;
		next_mcts->father = selected;

		// next_mcts->p_of_win = (double)next_mcts->situation->redValue/(next_mcts->situation->blackValue+next_mcts->situation->redValue);
		// if(next_mcts->situation->player==BLACK)
		// 	next_mcts->p_of_win = 1-p_of_win;
		// next_mcts->from_move.copyOneMove(to_new_situation);

		selected->tryed_choices.emplace_back(next_mcts);
		// Log().info(next_mcts->p_of_win);
		while (selected != initial_mcts)
		{
			selected->all_times += 1;
			selected->win_times += actual_result;
			selected->point_all += next_mcts->p_of_win;
			selected = selected->father;
		}

		selected->all_times += 1;
		selected->point_all += next_mcts->p_of_win;
		selected->win_times += actual_result;
	}
	// printFormctsDebug();
	printFormctsDebug2();
}

std::string mcts::getBestMoveString()
{
	double max_win_score = -1;
	if (tryed_choices.empty())
	{
		Move x("a0i9");
		// Log().info("error by feifei3");
		return x.moveToString();
	}
	int choice = 0;
	mcts *ans;
	if(best_move_after!=this)
		ans = best_move_after;
	else 
	{
		for (auto i : tryed_choices)
		{
			double p= i->point_all/i->all_times;
			if(initial_mcts->situation->player==BLACK) p= 1-p;
			double win_score = (double)i->win_times / i->all_times*0.3+p;
			unsigned int wintime = 0;
			if (win_score > max_win_score || win_score == max_win_score && (i->all_times > wintime || i->is_over))
			{
				max_win_score = win_score;
				wintime = i->all_times;
				ans = i;
			}
		}
		best_move_after = ans;
	}
	return ans->from_move.moveToString();
}
// Move &mcts::getBestMove()
// {
// 	// double max_win_score = -1
// 	int max_win_score = -1;
// 	if (tryed_choices.empty())
// 	{
// 		Move *x = new Move("a0i9");
// 		// Log().info("error by feifei3");
// 		return *x;
// 	}
// 	int choice = 0;
// 	int cnt = 0;
// 	for (auto i : tryed_choices)
// 	{
// 		// double win_score = (double)i->win_times / i->all_times
// 		int win_score = i->all_times;
// 		//std::cout << situation->mov[cnt].moveToString() << std::endl;
// 		//std::cout << win_score << std::endl;
// 		if (win_score > max_win_score)
// 		{
// 			max_win_score = win_score;
// 			choice = cnt;
// 		}
// 		cnt++;
// 	}
// 	return situation->mov[choice];
// }
mcts *mcts::new_world(Board *new_world_)
{
	for (auto i : best_move_after->tryed_choices)
	{
		unsigned char *tmp = new_world_->board;
		unsigned char *tmpw = i->situation->board;
		bool flag = 1;
		while (*tmp != '\0')
		{
			if (tmp++ != tmpw++)
			{
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			return i;
		}
	}
	mcts *ans = new mcts(new_world_);
	return ans;
}

void mcts::printFormctsDebug()
{
	std::cout << "all results" << std::endl;
	std::cout << win_times << " " << all_times << std::endl;
	mcts *now = this;
	int t = 0;
	while (t < tryed_choices.size())
	{
		std::cout << "tryed_choices:" << now->situation->mov[t].moveToString() << std::endl;
		std::cout << now->tryed_choices[t]->win_times << " " << now->tryed_choices[t]->all_times << std::endl;
		t++;
		std::cout << std::endl;
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
	Log().info("allresult::" + std::to_string(win_times) + ' ' + std::to_string(all_times));
	// std::cout << win_times << " " << all_times << std::endl;
	// mcts *now = this;
	// situation->printBoardForDebug();
	// Log().add(situation->player);
	Log().add(std::to_string(point_all/all_times));
	Log().add(point_all);
	for(auto t : tryed_choices)
	{
		// Log().add(t->situation->player);
		// std::cout<<"tryed_choices:"<<now->situation->mov[t].moveToString()<<std::endl;
		Log().add("tryed_choices:" + t->from_move.moveToString());
		// std::cout << now->tryed_choices[t]->win_times << " " << now->tryed_choices[t]->all_times << std::endl;
		Log().add("result::" + std::to_string(t->win_times) + " " + std::to_string(t->all_times));
		// std::cout<<std::endl;
		// t->situation->printBoardForDebug();
		Log().add(std::to_string(t->point_all/t->all_times));
		// Log().add(std::to_string(t->point_all));
	}
}