#include <iostream>

#include "cartridge.hpp"

int main(int argc, char** argv){

    if(argc < 2) {
        std::cout << "[-] You must pass a game path" << std::endl;
        return -1;
    }

    std::cout << "[+] NES emulator starting" << std::endl;

    Cartridge game_cartridge(argv[1]);


    return 0;
}