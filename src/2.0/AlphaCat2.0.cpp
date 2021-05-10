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
#include "log.h"
#include "ucci.h"
#include "game.h"
#include "pregen.h"
#include "move.h"

int main(int argc, char* argv[])
{
    preGenInit();
    Game test;
    test.buildFromFen("9/9/3k5/9/9/9/4R4/3A5/8r/4K4");
    test.printForDebug();
    Move move[120];
    int num = test.genNonCapMoves(move);
    for (int i = 0; i < num; i++)
    {
        char ch = getchar();
        test.takeOneMove(move[i].step);
        test.printForDebug();
        test.buildFromFen("9/9/3k5/9/9/9/4R4/3A5/8r/4K4");
    }
    
    return 0;
}