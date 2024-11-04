#include <iostream>

#include "cpu6502.hpp"

CPU6502::CPU6502(BUS* bus) {
        std::cout << "[+] Setting up the CPU" << std::endl;
        m_bus = bus;
        read_reset_vector();
}

void CPU6502::read_reset_vector() {
    uint16_t  reset_vector_address = 0;
    uint8_t   byte1 = m_bus->read(0xFFFC);
    uint8_t   byte2 = m_bus->read(0xFFFD);

    reset_vector_address &= byte1;
    reset_vector_address <<= 8;
    reset_vector_address &= byte2;

    m_registers.PC = reset_vector_address;
}

uint64_t CPU6502::get_cycle_count() {
    return m_cycles;
}