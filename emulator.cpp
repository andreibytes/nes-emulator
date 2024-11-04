#include <iostream>

#include "cartridge.hpp"
#include "bus.hpp"
#include "cpu6502.hpp"

int main(int argc, char** argv){

    if(argc < 2) {
        std::cout << "[-] You must pass a game path" << std::endl;
        return -1;
    }

    std::cout << "[+] NES emulator starting" << std::endl;

    Cartridge game_cartridge(argv[1]);

    if(!game_cartridge.get_load_status()){
        return -1;
    }

    BUS bus;
    CPU6502 cpu(&bus);


    return 0;
}