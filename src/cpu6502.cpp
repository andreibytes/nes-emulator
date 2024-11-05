#include <iostream>

#include "cpu6502.hpp"

CPU6502::CPU6502(BUS* bus) {
        std::cout << "[+] Setting up the CPU" << std::endl;
        m_bus = bus;
        read_reset_vector();
        setup_call_table();
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

uint64_t CPU6502::get_cycle_count() const {
    return m_cycles;
}

void CPU6502::set_addressing_mode(uint8_t opcode){
    if(opcode == 0x00 || opcode == 0x18 || opcode == 0xD8 || opcode == 0x58 ||
    opcode == 0xB8 || opcode == 0xCA || opcode == 0x88 || opcode == 0xE8 || opcode == 0xC8 ||
    opcode == 0xEA || opcode == 0x48 || opcode == 0x08 || opcode == 0x68 || opcode == 0x28 || 
    opcode == 0x40 || opcode == 0x60 || opcode == 0x38 || opcode == 0xF8 || opcode == 0x78 ||
    opcode == 0xAA || opcode == 0xA8 || opcode == 0xBA || opcode == 0x8A || opcode == 0x9A ||
    opcode == 0x98) {
        m_current_addressing_mode = IMPLIED;
    } else if(opcode == 0x0A || opcode == 0x4A || opcode == 0x2A || opcode == 0x6A) {
        m_current_addressing_mode = ACCUMULATOR;
    } else if(opcode == 0x69 || opcode == 0x29 || opcode == 0xC9 || opcode == 0x58 ||
    opcode == 0xE0 || opcode == 0xC0 || opcode == 0x49 || opcode == 0xA9 || opcode == 0xA2 ||
    opcode == 0xA0 || opcode == 0x09 || opcode == 0xE9) {
        m_current_addressing_mode = IMMEDIATE;
    } else if(opcode == 0x6D || opcode == 0x2D || opcode == 0x0E || opcode == 0x2C ||
    opcode == 0xCD || opcode == 0xEC || opcode == 0xCC || opcode == 0xCE || opcode == 0x4D ||
    opcode == 0xEE || opcode == 0x4C || opcode == 0x20 || opcode == 0xAD || opcode == 0xAE || 
    opcode == 0xAC || opcode == 0x4E || opcode == 0x0D || opcode == 0x2E || opcode == 0x6E ||
    opcode == 0xED || opcode == 0x8D || opcode == 0x8E || opcode == 0x8C) {
        m_current_addressing_mode = ABSOLUTE;
    } else if(opcode == 0x7D || opcode == 0x3D || opcode == 0x1E || opcode == 0xDD ||
    opcode == 0xDE || opcode == 0x5D || opcode == 0xFE || opcode == 0xBD || opcode == 0xBC ||
    opcode == 0x5E || opcode == 0x1D || opcode == 0x3E || opcode == 0x7E || opcode == 0xFD || 
    opcode == 0x9D) {
        m_current_addressing_mode = ABSOLUTEX;
    }  else if(opcode == 0x79 || opcode == 0x39 || opcode == 0xD9 || opcode == 0x59 ||
    opcode == 0xB9 || opcode == 0xBE || opcode == 0x19 || opcode == 0xF9 || opcode == 0x99) {
        m_current_addressing_mode = ABSOLUTEY;
    }  else if (opcode == 0x6C) {
        m_current_addressing_mode = INDIRECT;
    } else if(opcode == 0x65 || opcode == 0x25 || opcode == 0x06 || opcode == 0x24 ||
    opcode == 0xC5 || opcode == 0xE4 || opcode == 0xC4 || opcode == 0xC6 || opcode == 0x45 ||
    opcode == 0xE6 || opcode == 0xA5 || opcode == 0xA6 || opcode == 0xA4 || opcode == 0x46 || 
    opcode == 0x05 || opcode == 0x26 || opcode == 0x66 || opcode == 0xE5 || opcode == 0x85 ||
    opcode == 0x86 || opcode == 0x84) {
        m_current_addressing_mode = ZEROPAGE;
    } else if(opcode == 0x75 || opcode == 0x35 || opcode == 0x16 || opcode == 0xD5 ||
    opcode == 0xD6 || opcode == 0x55 || opcode == 0xF6 || opcode == 0xB5 || opcode == 0xB4 ||
    opcode == 0x56 || opcode == 0x15 || opcode == 0x36 || opcode == 0x76 || opcode == 0xF5 || 
    opcode == 0x95 || opcode == 0x94) {
        m_current_addressing_mode = ZEROPAGEX;
    } else if (opcode == 0xB6 || opcode == 0x96) {
        m_current_addressing_mode = ZEROPAGEY;
    } else if(opcode == 0x61 || opcode == 0x21 || opcode == 0xC1 || opcode == 0x41 ||
    opcode == 0xA1 || opcode == 0x01 || opcode == 0xE1 || opcode == 0x81) {
        m_current_addressing_mode = INDIRECTX;
    } else if(opcode == 0x71 || opcode == 0x31 || opcode == 0xD1 || opcode == 0x51 ||
    opcode == 0xB1 || opcode == 0x11 || opcode == 0xF1 || opcode == 0x91) {
        m_current_addressing_mode = INDIRECTY;
    } else if(opcode == 0x90 || opcode == 0xB0 || opcode == 0xF0 || opcode == 0x30 ||
    opcode == 0xD0 || opcode == 0x10 || opcode == 0x50 || opcode == 0x70) {
        m_current_addressing_mode = RELATIVE;
    } 
}

void CPU6502::setup_call_table(){
    m_call_table[0x69] = ADC; m_call_table[0x6D] = ADC; m_call_table[0x7D] = ADC;  m_call_table[0x79] = ADC;
    m_call_table[0x65] = ADC; m_call_table[0x75] = ADC; m_call_table[0x61] = ADC;  m_call_table[0x71] = ADC;
    m_call_table[0x29] = AND; m_call_table[0x2D] = AND; m_call_table[0x3D] = AND;  m_call_table[0x39] = AND; 
    m_call_table[0x25] = AND; m_call_table[0x35] = AND; m_call_table[0x21] = AND;  m_call_table[0x31] = AND; 
    m_call_table[0x0A] = ASL; m_call_table[0x0E] = ASL; m_call_table[0x1E] = ASL;  m_call_table[0x06] = ASL; 
    m_call_table[0x16] = ASL; m_call_table[0x90] = BCC; m_call_table[0xB0] = BCS;  m_call_table[0xF0] = BEQ;
    m_call_table[0x2C] = BIT; m_call_table[0x24] = BIT; m_call_table[0x30] = BMI;  m_call_table[0xD0] = BNE;
    m_call_table[0x10] = BPL; m_call_table[0x00] = BRK; m_call_table[0x50] = BVC;  m_call_table[0x70] = BVS;
    m_call_table[0x18] = CLC; m_call_table[0xD8] = CLD; m_call_table[0x58] = CLI;  m_call_table[0xB8] = CLV;
    m_call_table[0xC9] = CMP; m_call_table[0xCD] = CMP; m_call_table[0xDD] = CMP;  m_call_table[0xD9] = CMP;
    m_call_table[0xC5] = CMP; m_call_table[0xD5] = CMP; m_call_table[0xC1] = CMP;  m_call_table[0xD1] = CMP;
    m_call_table[0xE0] = CPX; m_call_table[0xEC] = CPX; m_call_table[0xE4] = CPX;  m_call_table[0xC0] = CPY; 
    m_call_table[0xCC] = CPY; m_call_table[0xC4] = CPY; m_call_table[0xCE] = DEC;  m_call_table[0xDE] = DEC; 
    m_call_table[0xC6] = DEC; m_call_table[0xD6] = DEC; m_call_table[0xCA] = DEX;  m_call_table[0x88] = DEY;
    m_call_table[0x49] = EOR; m_call_table[0x4D] = EOR; m_call_table[0x5D] = EOR;  m_call_table[0x59] = EOR;
    m_call_table[0x45] = EOR; m_call_table[0x55] = EOR; m_call_table[0x41] = EOR;  m_call_table[0x51] = EOR;
    m_call_table[0xEE] = INC; m_call_table[0xFE] = INC; m_call_table[0xE6] = INC;  m_call_table[0xF6] = INC;
    m_call_table[0xE8] = INX; m_call_table[0xC8] = INY; m_call_table[0x4C] = JMP;  m_call_table[0x6C] = JMP;
    m_call_table[0x20] = JSR; m_call_table[0xA9] = LDA; m_call_table[0xAD] = LDA;  m_call_table[0xBD] = LDA; 
    m_call_table[0xB9] = LDA; m_call_table[0xA5] = LDA; m_call_table[0xB5] = LDA;  m_call_table[0xA1] = LDA; 
    m_call_table[0xB1] = LDA; m_call_table[0xA2] = LDX; m_call_table[0xAE] = LDX;  m_call_table[0xBE] = LDX; 
    m_call_table[0xA6] = LDX; m_call_table[0xB6] = LDX; m_call_table[0xA0] = LDY;  m_call_table[0xAC] = LDY; 
    m_call_table[0xBC] = LDY; m_call_table[0xA4] = LDY; m_call_table[0xB4] = LDY;  m_call_table[0x4A] = LSR; 
    m_call_table[0x4E] = LSR; m_call_table[0x5E] = LSR; m_call_table[0x46] = LSR;  m_call_table[0x56] = LSR;
    m_call_table[0xEA] = NOP; m_call_table[0x09] = ORA; m_call_table[0x0D] = ORA;  m_call_table[0x1D] = ORA; 
    m_call_table[0x19] = ORA; m_call_table[0x05] = ORA; m_call_table[0x15] = ORA;  m_call_table[0x01] = ORA; 
    m_call_table[0x11] = ORA; m_call_table[0x48] = PHA; m_call_table[0x08] = PHP;  m_call_table[0x68] = PLA;
    m_call_table[0x28] = PLP; m_call_table[0x2A] = ROL; m_call_table[0x2E] = ROL;  m_call_table[0x3E] = ROL; 
    m_call_table[0x26] = ROL; m_call_table[0x36] = ROL; m_call_table[0x6A] = ROR;  m_call_table[0x6E] = ROR; 
    m_call_table[0x7E] = ROR; m_call_table[0x66] = ROR; m_call_table[0x76] = ROR;  m_call_table[0x40] = RTI;
    m_call_table[0x60] = RTS; m_call_table[0xE9] = SBC; m_call_table[0xED] = SBC;  m_call_table[0xFD] = SBC; 
    m_call_table[0xF9] = SBC; m_call_table[0xE5] = SBC; m_call_table[0xF5] = SBC;  m_call_table[0xE1] = SBC; 
    m_call_table[0xF1] = SBC; m_call_table[0x38] = SEC; m_call_table[0xF8] = SED;  m_call_table[0x78] = SEI;
    m_call_table[0x8D] = STA; m_call_table[0x9D] = STA; m_call_table[0x99] = STA;  m_call_table[0x85] = STA;
    m_call_table[0x95] = STA; m_call_table[0x81] = STA; m_call_table[0x91] = STA;  m_call_table[0x8E] = STX; 
    m_call_table[0x86] = STX; m_call_table[0x96] = STX; m_call_table[0x8C] = STY;  m_call_table[0x84] = STY; 
    m_call_table[0x94] = STY; m_call_table[0xAA] = TAX; m_call_table[0xA8] = TAY;  m_call_table[0xBA] = TSX;
    m_call_table[0x8A] = TXA; m_call_table[0x9A] = TXS; m_call_table[0x98] = TYA;
}