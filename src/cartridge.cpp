#include "cartridge.h"

#include <iostream>
#include <fstream>

void Cartridge::load(std::string rom_path) {
    std::cout << "info: Attempting to load " << rom_path << std::endl;

    std::ifstream rom_file;
    rom_file.open(rom_path);

    if(!rom_file.is_open()) {
        std::cout << "error: to open rom file" << std::endl;
        return;
    } 

    uint8_t header[16];
    int header_index = 0;

    // Parse INES header
    for(; header_index < 16 && !rom_file.eof(); header_index++) {
        header[header_index] = rom_file.get();
    }

    if(header_index < 15) {
        std::cout << "error: file header invalid" << std::endl;
        return;
    }

    // Verify INES header
    if(header[0] == 0x4E && header[1] == 0x45 && header[2] == 0x53 && header[3] == 0x1A) {
        // Allocate program-rom and chr-rom
        m_program_rom_size = header[4] * KB * 16;
        m_chr_rom_size = header[5] * KB * 8;

        m_program_rom = std::make_unique<uint8_t[]>(m_program_rom_size);
        m_chr_rom = std::make_unique<uint8_t[]>(m_chr_rom_size);

        m_mapper_id = header[6];

        // TODO: Implement function which returns enum that represents the name of the mapper so I can print it to the terminal

        // Transfer the program-rom and chr-rom data from the rom file to the Cartridge
        for(uint32_t i = 0; i < m_program_rom_size; i++) {
            m_program_rom[i] = rom_file.get();
        }

        for(uint32_t i = 0; i < m_chr_rom_size; i++) {
            m_chr_rom[i] = rom_file.get();
        }

        std::cout << "info: loaded  " << rom_path << std::endl;
        std::cout << "info: mapper_id=" << m_mapper_id << std::endl;

        m_load_status = true;

    } else {
        std::cout << "error: not a valid INES file" << std::endl;
        return;
    }
}