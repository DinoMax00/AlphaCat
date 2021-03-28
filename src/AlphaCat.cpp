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
    // std::cout << AlphaCat.board.mtcsMove();
    return 0;
}