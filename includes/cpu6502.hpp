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
        using FuncPtr = void(CPU6502::*)();
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
        std::unordered_map<uint8_t, FuncPtr> m_call_table;
        e_addressing_mode m_current_addressing_mode = IMMEDIATE;
    private:
        void read_reset_vector();
        void setup_call_table();
        void set_addressing_mode(uint8_t opcode);
    private:
        void LDA(); void LDX(); void LDY(); void STA();
        void STX(); void STY(); void TAX(); void TAY();
        void TXA(); void TYA(); void TSX(); void TXS();
        void ADC(); void SBC(); void AND(); void ORA();
        void EOR(); void CMP(); void CPX(); void CPY();
        void INC(); void INX(); void INY(); void DEC();
        void DEX(); void DEY(); void LSR(); void ASL();
        void ROL(); void ROR(); void BIT(); void BCC();
        void BCS(); void BEQ(); void BMI(); void BNE();
        void BPL(); void BVC(); void BVS(); void JMP();
        void JSR(); void RTS(); void CLC(); void CLD();
        void CLI(); void CLV(); void SEC(); void SED();
        void SEI(); void PHA(); void PHP(); void PLA();
        void PLP(); void NOP(); void BRK(); void RTI();
};