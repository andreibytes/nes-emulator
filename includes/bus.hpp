#pragma once

#include "common.hpp"

class BUS {
    public:
        BUS();
        uint8_t read(uint16_t address);
        void    write(uint16_t address, uint8_t data);
    private:
        uint8_t m_cpu_mem[2 * KB];
};