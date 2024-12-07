#pragma once

#include <string>

#include "cartridge.h"
#include "bus.h"
#include "processor.h"

namespace emulator {
    Cartridge initialize_cartridge();
    void load(Cartridge& cartridge, std::string rom_path);
    void start(std::string rom_path);
}