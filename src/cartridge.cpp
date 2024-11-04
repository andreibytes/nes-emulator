#include <iostream>
#include <fstream>

#include "cartridge.hpp"


Cartridge::Cartridge(std::string game_path){
    m_game_path = game_path;
    load_game();
}

void Cartridge::load_game(){
    std::cout << "[+] Attempting to load " << m_game_path << std::endl;

    std::ifstream game_file;
    game_file.open(m_game_path);

    if(!game_file.is_open()){
        std::cout << "[-] Failed to open game file" << std::endl;
        m_file_load_success = false;
    } else {
        // Parse header
        
        uint8_t header[16];
        int header_index = 0;

        for(; header_index < 16 && !game_file.eof(); header_index++){
            header[header_index] = game_file.get();
        }

        if(header_index < 15) {
            std::cout << "[-] File header couldn't not be read" << std::endl;
            m_file_load_success = false;
        } else {
            // Verify this is an INES file
            if(header[0]==0x4E && header[1]== 0x45 && header[2]== 0x53 && header[3]==0x1A) {
                // Allocate prg-rom and chr-rom

                m_prg_rom_size = header[4] * KB16;
                m_chr_rom_size = header[5] * KB8;

                m_prg_rom = std::make_unique<uint8_t[]>(m_prg_rom_size);
                m_chr_rom = std::make_unique<uint8_t[]>(m_chr_rom_size);
                

                m_mapper_id = header[5];

                // Write the cartridge data to m_prg_rom and m_chr_rom, this piece of code assumes that no trainers are present
                for(uint32_t i = 0; i < m_prg_rom_size; i++) {
                    m_prg_rom[i] = game_file.get();
                }

                for(uint32_t i = 0; i < m_chr_rom_size; i++) {
                    m_chr_rom[i] = game_file.get();
                }

                std::cout << "[+] Loaded " << m_game_path << std::endl;

            } else {
                std::cout << "[-] Not a valid INES file, please try again" << std::endl;
                m_file_load_success = false;
            }
        }

        game_file.close();
    }


}


uint8_t Cartridge::get_mapper_id() const {
    return m_mapper_id;
}


bool Cartridge::get_load_status()  const {
    return m_file_load_success;
}