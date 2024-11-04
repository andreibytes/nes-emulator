#pragma once

#include <string>
#include <memory>

#include "common.hpp"
#include "mapper.hpp"

class Cartridge {
    public:
        Cartridge(std::string game_path);
        uint8_t get_mapper_id() const;
        bool get_load_status()  const;
    private:
        std::unique_ptr<uint8_t[]> m_prg_rom;
        std::unique_ptr<uint8_t[]> m_chr_rom;
        std::string m_game_path;
        uint8_t m_mapper_id = 0;
        uint32_t m_prg_rom_size = 0;
        uint32_t m_chr_rom_size = 0;
        bool m_file_load_success = true;
    private:
        void load_game();
};