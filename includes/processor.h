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
        std::unordered_map<uint8_t, void(Processor::*)()> m_call_table;
        e_addressing_mode m_addressing_mode;
    // Methods for initializing the cpu
    private:
        void read_reset_vector();
        void setup_call_table();
        void set_addressing_mode(uint8_t opcode);
        uint8_t fetch();
        void fetch_opcode();
        void execute_read_modify_write(std::function<void(uint8_t&)> modify);
        void execute_store(std::function<void(uint8_t)> write);
    
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