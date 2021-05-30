/*****************************************************************//**
 * @file   AlphaCat2.0.cpp
 * @brief  AlphaCat³ÌÐòÈë¿Ú
 *
 * @license: MIT
 * @copyright: Copyright 2021 TongJi
 * @birth: created by Dino FeiFei PPZ AYi on 2021-xx-xx
 *********************************************************************/

#include <iostream>
#include <ctime>
#include <bitset>

#include "ucci.h"
#include "game.h"
#include "pregen.h"
#include "move.h"
#include "hash.h"

int main(int argc, char* argv[])
{
	preGenInit();
	newTable();

	UcciEngine* alpha_cat = new UcciEngine();

	alpha_cat->bootEngine();
	alpha_cat->run();

	delTable();
	return 0;
}