#pragma once
 
#include <memory>
#include <string>

#include "shared.h"

class Cartridge {
    public:
        void load(std::string rom_path);
    private:
        uint8_t m_program_rom_size = 0;
        uint8_t m_chr_rom_size = 0;
        uint8_t m_mapper_id = 0;
        bool m_load_status = false;
        std::unique_ptr<uint8_t[]> m_program_rom;
        std::unique_ptr<uint8_t[]> m_chr_rom;
};