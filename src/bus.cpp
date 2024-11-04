#include <iostream>

#include "bus.hpp"

BUS::BUS(){
    std::cout << "[+] Setting up the BUS" << std::endl;
}


uint8_t BUS::read(uint16_t address){
    return m_cpu_mem[address & CPU_MEM_MASK];
}

void BUS::write(uint16_t address, uint8_t data){
    m_cpu_mem[address & CPU_MEM_MASK] = data;
}