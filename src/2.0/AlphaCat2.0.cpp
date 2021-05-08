/*****************************************************************//**
 * @file   AlphaCat2.0.cpp
 * @brief  AlphaCat³ÌÐòÈë¿Ú
 *
 * @license: MIT
 * @copyright: Copyright 2021 TongJi
 * @birth: created by Dino FeiFei PPZ AYi on 2021-xx-xx
 * @version V0.0.0.3
 *********************************************************************/
#include <iostream>
#include <ctime>
#include "log.h"
#include "ucci.h"
#include "game.h"

int main(int argc, char* argv[])
{
    Game test;
    test.buildFromFen("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR");
    test.printForDebug();
    test.takeOneMove(stringToMove(moveToString(stringToMove("b2b9"))));
    test.printForDebug();
    test.printForDebug();
    return 0;
}