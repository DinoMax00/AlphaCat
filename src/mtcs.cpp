#include"mtcs.h"
#include<math.h>
#include"log.h"
Mtcs::Mtcs(Board* bo)
{
	situation = bo;
	initial_mtcs = this;
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


void Mtcs::selectionOfTry()
{
	int i = 0;
	while (i++ < MTCS_TIMES) 
	{
		//printForMtcsDebug();
		//std::cout << situation->player << std::endl;
		Mtcs* selected = this;
		//std::cout << ">>fuck" << std::endl;
		if (selected->situation->mov.empty() || selected->is_over) Log().info("error by feifei");
		while (selected->tryed_choices.size() == selected->situation->mov.size())
		{
			//std::cout << ">>fuck" << std::endl;
			double max_score = -1;
			Mtcs* next = selected;
			for (auto i : selected->tryed_choices)
			{
				if (i->situation->mov.empty() || i->is_over) continue;
				double score = -10000;
				if (selected->situation->player == initial_mtcs->situation->player)
					score = (double)i->win_times / i->all_times + MTCS_C * sqrt(log(selected->all_times) / i->all_times);
				else
					score = ((double)i->all_times - i->win_times) / i->all_times + MTCS_C * sqrt(log(selected->all_times) / i->all_times);
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
			if (selected == initial_mtcs)
				break;
		}
		if (selected == initial_mtcs && selected->tryed_choices.size() == selected->situation->mov.size())
		{
			Log().info("error by feifei2");
			std::cout << "??" << std::endl;
			return;
		}
		Move& to_new_situation = selected->situation->mov[selected->tryed_choices.size()];
		Board* next_board = new Board(selected->situation, to_new_situation);

		Board* board_play = new Board(selected->situation, to_new_situation);
		int result = board_play->mtcsMove();

		Mtcs* next_mtcs = new Mtcs(next_board);
		int x;
		//unsigned int result = situation->mtcsMove();
		if (result <= 0 && next_board->player == initial_mtcs->situation->player || result > 0 && next_board->player != initial_mtcs->situation->player)
			x=next_mtcs->win_times = 0;
		else
			x=next_mtcs->win_times = 1;
		
		next_mtcs->initial_mtcs = initial_mtcs;
		next_mtcs->father = selected;
		selected->tryed_choices.emplace_back(next_mtcs);

		next_mtcs->win_times;
		
		while (selected != initial_mtcs) 
		{
			 selected->all_times++;
			 selected->win_times += x;
			 selected = selected->father;
		}

		selected->all_times++;
		selected->win_times += x;
	}
	
}


Move& Mtcs::getBestMove()
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

std::string Mtcs::getBestMoveString()
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
		double win_score = (double)i->win_times / i->all_times;
		//std::cout << situation->mov[cnt].moveToString() << std::endl;
		//std::cout << i->win_times<<' '<<i->all_times << std::endl;
		//std::cout << win_score << std::endl;
		//std::cout << std::endl;
		if (win_score > max_win_score)
		{
			max_win_score = win_score;
			choice = cnt;
		}
		cnt++;
	}
	return situation->mov[choice].moveToString();
}

void Mtcs::printForMtcsDebug() 
{
	std::cout << win_times << " " << all_times << std::endl;
	std::cout << tryed_choices.size()<<" "<<this->situation->mov.size()<<std::endl;
	for (int i = 0; i < (int)tryed_choices.size(); i++) 
	{
		std::cout << situation->mov[i].moveToString() << std::endl;
		std::cout << tryed_choices[i]->win_times << " " << tryed_choices[i]->all_times << std::endl;
	}
	//this->situation->printBoardForDebug2();
	std::cout << std::endl;
	/*for (auto i : tryed_choices) 
	{
		std::cout << i->win_times << " " << i->all_times << std::endl;
		std::cout << i->tryed_choices.size() << std::endl;
		std::cout << std::endl;
	}*/
}