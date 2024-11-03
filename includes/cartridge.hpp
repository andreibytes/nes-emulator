#pragma once

#include <string>
#include <memory>

#include "common.hpp"

class Cartridge {
    public:
        Cartridge(std::string game_path);
    private:
        std::unique_ptr<uint8_t[]> m_prg_rom;
        std::unique_ptr<uint8_t[]> m_chr_rom;
        std::string m_game_path;
        uint8_t m_mapper_id = 0;
        uint32_t m_prg_rom_size = 0;
        uint32_t m_chr_rom_size = 0;
    private:
        void load_game();
};