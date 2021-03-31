/*****************************************************************//**
 * \file   AlphaCat.cpp
 * \brief  AlphaCat³ÌÐòÈë¿Ú
 * 
 * \license: MIT
 * \copyright: Copyright 2021 TongJi
 * \birth: created by Dino FeiFei PPZ AYi on 2021-xx-xx
 * \version V0.0.3
 *********************************************************************/
#include <iostream>
#include <ctime>
#include "log.h"
#include "ucci.h"

int main(int argc, char* argv[])
{
    Log().clear();
    srand((unsigned)time(NULL));
    UcciEngine AlphaCat;
    AlphaCat.bootEngine();
    AlphaCat.run();
    return 0;
}