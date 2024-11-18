#pragma once

#include <unordered_map>
#include <functional>

#include "bus.h"

typedef enum {
    ACCUMULATOR, ABSOLUTE, ABSOLUTEX, ABSOLUTEY,
    IMMEDIATE,   IMPLIED,  INDIRECT,  INDIRECTX,
    INDIRECTY,   RELATIVE, ZEROPAGE,  ZEROPAGEX,
    ZEROPAGEY
} e_addressing_mode;

// Class representing the modified 6502 processor on the NES

class Processor {
    public:
        Processor(Bus& bus);
    private:
        Bus& m_bus;
        struct {
            uint16_t PC;
            uint8_t  SP;
            uint8_t  AC;
            uint8_t  IX;
            uint8_t  IY;
            uint8_t  S;
        } m_registers;
        std::unordered_map<uint8_t, std::function<void(void)>> m_call_table;
        e_addressing_mode m_addressing_mode;
    // Methods for initializing the cpu
    private:
        void read_reset_vector();
        void set_addressing_mode(uint8_t opcode);
    
    // Methods for the cpu instructions
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