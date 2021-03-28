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
	Mtcs* selected = this;
	if (selected->situation->mov.empty() || selected->is_over) Log().info("error by feifei");
	while (selected->tryed_choices.size() == selected->situation->mov.size())
	{
		double max_score = -1;
		Mtcs* next = selected;
		for (auto i : selected->tryed_choices)
		{
			if (i->situation->mov.empty() || i->is_over) continue;
			double score = -10000;
			if (selected->situation->player == initial_mtcs->situation->player)
				score = i->win_times / i->all_times + MTCS_C * sqrt(log(selected->all_times) / i->all_times);
			else
				score = (i->all_times - i->win_times) / i->all_times + MTCS_C * sqrt(log(selected->all_times) / i->all_times);
			if (score > max_score)
			{
				max_score = score;
				next = i;
			}
		}
		if (next == selected)
		{
			selected->is_over = true;
			selected = selected->father;
		}
		else
			selected = next;
	}
	if (selected == initial_mtcs && selected->tryed_choices.size() == selected->situation->mov.size())
	{
		Log().info("error by feifei2");
		return;
	}
	Move& to_new_situation = selected->situation->mov[tryed_choices.size()];
	Board* next_board = new Board(selected->situation, to_new_situation);
	Mtcs* next_mtcs = new Mtcs(next_board);
	next_mtcs->initial_mtcs = initial_mtcs;
	next_mtcs->father = selected;
	tryed_choices.emplace_back(next_mtcs);
	int result = next_mtcs->win_times;
	do
	{
		selected->all_times++;
		selected->win_times += result;
		selected = selected->father;
	} while (selected != initial_mtcs);
}


Move& Mtcs::getBestMove()
{
	double max_win_score = -1;
	if (tryed_choices.empty())
	{
		Move x("a0i9");
		Log().info("error by feifei3");
		return x;
	}
	int choice = 0;
	int cnt = 0;
	for (auto i : tryed_choices)
	{
		double win_score = i->win_times / i->all_times;
		if (win_score > max_win_score)
			choice = cnt;
		cnt++;
	}
	return situation->mov[choice];
}

void Mtcs::printForMtcsDebug() 
{
	std::cout << win_times << " " << all_times << std::endl;
	std::cout << tryed_choices.size()<<std::endl;
	std::cout << std::endl;
	for (auto i : tryed_choices) 
	{
		std::cout << i->win_times << " " << i->all_times << std::endl;
		std::cout << i->tryed_choices.size() << std::endl;
		std::cout << std::endl;
	}
}