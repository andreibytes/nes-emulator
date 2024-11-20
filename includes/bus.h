#pragma once

#include "shared.h"

class Bus {
    public:
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);
    private:
        void tick();
        uint8_t m_cpu_mem[2 * KB];
};