#include "system.h"

Cartridge emulator::initialize_cartridge() {
    Cartridge game_cartridge;
    return game_cartridge;
}

void emulator::load(Cartridge& cartridge, std::string rom_path){
    cartridge.load(rom_path);
}

void emulator::start(std::string rom_path){
    Cartridge game_cartridge = initialize_cartridge();
    load(game_cartridge, rom_path);
}