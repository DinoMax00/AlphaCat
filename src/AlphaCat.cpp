#include <iostream>
#include <ctime>
#include "ucci.h"

int main(int argc, char* argv[])
{
    srand(time(NULL));
    UcciEngine AlphaCat;
    AlphaCat.bootEngine();
    AlphaCat.run();
    return 0;
}