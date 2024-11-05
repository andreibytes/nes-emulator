#pragma once

#include <unordered_map>
#include <functional>
#include "common.hpp"
#include "bus.hpp"


typedef enum {
    ACCUMULATOR, ABSOLUTE, ABSOLUTEX, ABSOLUTEY,
    IMMEDIATE,   IMPLIED,  INDIRECT,  INDIRECTX,
    INDIRECTY,   RELATIVE, ZEROPAGE,  ZEROPAGEX,
    ZEROPAGEY
} e_addressing_mode;

class CPU6502 {
    public:
        CPU6502(BUS* bus);
        uint64_t get_cycle_count() const;
    private:
        BUS* m_bus = nullptr;
        struct {
            uint16_t PC;
            uint8_t  SP;
            uint8_t  AC;
            uint8_t  IX;
            uint8_t  IY;
            uint8_t  S;
        } m_registers;
        uint64_t m_cycles = 0;
        std::unordered_map<uint8_t, std::function<void()>> m_call_table;
        e_addressing_mode m_current_addressing_mode = IMMEDIATE;
    private:
        void read_reset_vector();
        void setup_call_table();
        void set_addressing_mode(uint8_t opcode);
};