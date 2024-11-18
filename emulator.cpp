#include <iostream>

#include "system.h"


int main(int argc, char** argv){
    std::cout << "info: emulator starting" << std::endl;

    if(argc < 2){
        std::cout << "error: expecting more arguments" << std::endl;
        return -1;
    }

    emulator::start(argv[1]);

    return 0;
}