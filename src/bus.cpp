#include "bus.h"

uint8_t Bus::read(uint16_t address) {
    tick();
    if(address >= 0x0 && address <= 0x1FFF) {
        return m_cpu_mem[address & CPU_MEMORY_MASK];
    }
    // TODO: Implement reads for the PPU and other devices later on 
}

void Bus::write(uint16_t address, uint8_t data) {
    tick();
    if(address >= 0x0 && address <= 0x1FFF) {
        m_cpu_mem[address & CPU_MEMORY_MASK] = data;
    }

    // TODO: Implement writes for the PPU and other devices later on
}


void Bus::tick() {
     for(int i = 0; i < 3; i++){
        // TODO: Tick the PPU once I actually write it
    }
}
