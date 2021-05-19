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

#include "log.h"
#include "ucci.h"
#include "game.h"
#include "pregen.h"
#include "move.h"

int main(int argc, char* argv[])
{
    preGenInit();
    Game test;
    MoveSort move_sort;
    test.buildFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
    test.evaBoard();
    std::cout << test.red_val << " " << test.black_val << std::endl;
    test.printForDebug();
    move_sort.getAllMoves(test);
    while(true)
    {   
        char ch = getchar();
        if (test.takeOneMove(move_sort.next())) {
            test.printForDebug();
            std::cout << test.red_val << " " << test.black_val << std::endl;
            test.deleteOneMove();
            std::cout << test.red_val << " " << test.black_val << std::endl;
        }
    }
    char ch = getchar();
    return 0;
}