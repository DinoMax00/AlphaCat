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
    test.buildFromFen("rnbakabnr/9/1c5c1/9/9/9/9/1C5C1/9/RNBAKABNR");
    test.printForDebug();
    Move move[120];
    int num = test.genCapMoves(move);
    for (int i = 0; i < num; i++)
    {
        char ch = getchar();
        test.takeOneMove(move[i].step);
        test.printForDebug();
        test.buildFromFen("rnbakabnr/9/1c5c1/9/9/9/9/1C5C1/9/RNBAKABNR");
    }
    getchar();
    return 0;
}