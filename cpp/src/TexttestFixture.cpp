#include "TexttestSimulator.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char** argv) {
    int days = 2;
    if (argc > 1) {
        days = std::atoi(argv[1]);
    }

    std::cout << runTexttestSimulation(days);
    return 0;
}
