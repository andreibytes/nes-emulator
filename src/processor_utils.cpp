#include "processor.h"

#include <unordered_set>
#include <iostream>


void Processor::dummy_read(uint16_t address) {
    uint8_t data = m_bus.read(address);
}

void Processor::set_addressing_mode(uint8_t opcode) {
     static const std::unordered_set<uint8_t> IMPLIED_OPCODES = {
        0x00, 0x18, 0xD8, 0x58, 0xB8, 0xCA, 0x88, 0xE8,
        0xC8, 0xEA, 0x48, 0x08, 0x68, 0x28, 0x40, 0x60,
        0x38, 0xF8, 0x78, 0xAA, 0xA8, 0xBA, 0x8A, 0x9A, 0x98
    };
    
    static const std::unordered_set<uint8_t> ACCUMULATOR_OPCODES = {
        0x0A, 0x4A, 0x2A, 0x6A
    };
    
    static const std::unordered_set<uint8_t> IMMEDIATE_OPCODES = {
        0x69, 0x29, 0xC9, 0xE0, 0xC0, 0x49, 0xA9, 0xA2,
        0xA0, 0x09, 0xE9
    };
    
    static const std::unordered_set<uint8_t> ABSOLUTE_OPCODES = {
        0x6D, 0x2D, 0x0E, 0x2C, 0xCD, 0xEC, 0xCC, 0xCE,
        0x4D, 0xEE, 0x4C, 0x20, 0xAD, 0xAE, 0xAC, 0x4E,
        0x0D, 0x2E, 0x6E, 0xED, 0x8D, 0x8E, 0x8C
    };
    
    static const std::unordered_set<uint8_t> ABSOLUTEX_OPCODES = {
        0x7D, 0x3D, 0x1E, 0xDD, 0xDE, 0x5D, 0xFE, 0xBD,
        0xBC, 0x5E, 0x1D, 0x3E, 0x7E, 0xFD, 0x9D
    };
    
    static const std::unordered_set<uint8_t> ABSOLUTEY_OPCODES = {
        0x79, 0x39, 0xD9, 0x59, 0xB9, 0xBE, 0x19, 0xF9, 0x99
    };
    
    static const std::unordered_set<uint8_t> INDIRECT_OPCODES = {
        0x6C
    };
    
    static const std::unordered_set<uint8_t> ZEROPAGE_OPCODES = {
        0x65, 0x25, 0x06, 0x24, 0xC5, 0xE4, 0xC4, 0xC6,
        0x45, 0xE6, 0xA5, 0xA6, 0xA4, 0x46, 0x05, 0x26,
        0x66, 0xE5, 0x85, 0x86, 0x84
    };
    
    static const std::unordered_set<uint8_t> ZEROPAGEX_OPCODES = {
        0x75, 0x35, 0x16, 0xD5, 0xD6, 0x55, 0xF6, 0xB5,
        0xB4, 0x56, 0x15, 0x36, 0x76, 0xF5, 0x95, 0x94
    };
    
    static const std::unordered_set<uint8_t> ZEROPAGEY_OPCODES = {
        0xB6, 0x96
    };
    
    static const std::unordered_set<uint8_t> INDIRECTX_OPCODES = {
        0x61, 0x21, 0xC1, 0x41, 0xA1, 0x01, 0xE1, 0x81
    };
    
    static const std::unordered_set<uint8_t> INDIRECTY_OPCODES = {
        0x71, 0x31, 0xD1, 0x51, 0xB1, 0x11, 0xF1, 0x91
    };
    
    static const std::unordered_set<uint8_t> RELATIVE_OPCODES = {
        0x90, 0xB0, 0xF0, 0x30, 0xD0, 0x10, 0x50, 0x70
    };

    // Match opcode with the corresponding addressing mode
    if (IMPLIED_OPCODES.count(opcode)) {
        m_addressing_mode = IMPLIED;
    } else if (ACCUMULATOR_OPCODES.count(opcode)) {
        m_addressing_mode = ACCUMULATOR;
    } else if (IMMEDIATE_OPCODES.count(opcode)) {
        m_addressing_mode = IMMEDIATE;
    } else if (ABSOLUTE_OPCODES.count(opcode)) {
        m_addressing_mode = ABSOLUTE;
    } else if (ABSOLUTEX_OPCODES.count(opcode)) {
        m_addressing_mode = ABSOLUTEX;
    } else if (ABSOLUTEY_OPCODES.count(opcode)) {
        m_addressing_mode = ABSOLUTEY;
    } else if (INDIRECT_OPCODES.count(opcode)) {
        m_addressing_mode = INDIRECT;
    } else if (ZEROPAGE_OPCODES.count(opcode)) {
        m_addressing_mode = ZEROPAGE;
    } else if (ZEROPAGEX_OPCODES.count(opcode)) {
        m_addressing_mode = ZEROPAGEX;
    } else if (ZEROPAGEY_OPCODES.count(opcode)) {
        m_addressing_mode = ZEROPAGEY;
    } else if (INDIRECTX_OPCODES.count(opcode)) {
        m_addressing_mode = INDIRECTX;
    } else if (INDIRECTY_OPCODES.count(opcode)) {
        m_addressing_mode = INDIRECTY;
    } else if (RELATIVE_OPCODES.count(opcode)) {
        m_addressing_mode = RELATIVE;
    } else {
        // Handle invalid or unimplemented opcode
        std::cout << "Invalid or unimplemented opcode: " << opcode << std::endl;
    }
}


void Processor::setup_call_table(){
m_call_table[0x69] = &Processor::ADC; m_call_table[0x6D] = &Processor::ADC; m_call_table[0x7D] = &Processor::ADC;  m_call_table[0x79] = &Processor::ADC;
m_call_table[0x65] = &Processor::ADC; m_call_table[0x75] = &Processor::ADC; m_call_table[0x61] = &Processor::ADC;  m_call_table[0x71] = &Processor::ADC;
m_call_table[0x29] = &Processor::AND; m_call_table[0x2D] = &Processor::AND; m_call_table[0x3D] = &Processor::AND;  m_call_table[0x39] = &Processor::AND; 
m_call_table[0x25] = &Processor::AND; m_call_table[0x35] = &Processor::AND; m_call_table[0x21] = &Processor::AND;  m_call_table[0x31] = &Processor::AND; 
m_call_table[0x0A] = &Processor::ASL; m_call_table[0x0E] = &Processor::ASL; m_call_table[0x1E] = &Processor::ASL;  m_call_table[0x06] = &Processor::ASL; 
m_call_table[0x16] = &Processor::ASL; m_call_table[0x90] = &Processor::BCC; m_call_table[0xB0] = &Processor::BCS;  m_call_table[0xF0] = &Processor::BEQ;
m_call_table[0x2C] = &Processor::BIT; m_call_table[0x24] = &Processor::BIT; m_call_table[0x30] = &Processor::BMI;  m_call_table[0xD0] = &Processor::BNE;
m_call_table[0x10] = &Processor::BPL; m_call_table[0x00] = &Processor::BRK; m_call_table[0x50] = &Processor::BVC;  m_call_table[0x70] = &Processor::BVS;
m_call_table[0x18] = &Processor::CLC; m_call_table[0xD8] = &Processor::CLD; m_call_table[0x58] = &Processor::CLI;  m_call_table[0xB8] = &Processor::CLV;
m_call_table[0xC9] = &Processor::CMP; m_call_table[0xCD] = &Processor::CMP; m_call_table[0xDD] = &Processor::CMP;  m_call_table[0xD9] = &Processor::CMP;
m_call_table[0xC5] = &Processor::CMP; m_call_table[0xD5] = &Processor::CMP; m_call_table[0xC1] = &Processor::CMP;  m_call_table[0xD1] = &Processor::CMP;
m_call_table[0xE0] = &Processor::CPX; m_call_table[0xEC] = &Processor::CPX; m_call_table[0xE4] = &Processor::CPX;  m_call_table[0xC0] = &Processor::CPY; 
m_call_table[0xCC] = &Processor::CPY; m_call_table[0xC4] = &Processor::CPY; m_call_table[0xCE] = &Processor::DEC;  m_call_table[0xDE] = &Processor::DEC; 
m_call_table[0xC6] = &Processor::DEC; m_call_table[0xD6] = &Processor::DEC; m_call_table[0xCA] = &Processor::DEX;  m_call_table[0x88] = &Processor::DEY;
m_call_table[0x49] = &Processor::EOR; m_call_table[0x4D] = &Processor::EOR; m_call_table[0x5D] = &Processor::EOR;  m_call_table[0x59] = &Processor::EOR;
m_call_table[0x45] = &Processor::EOR; m_call_table[0x55] = &Processor::EOR; m_call_table[0x41] = &Processor::EOR;  m_call_table[0x51] = &Processor::EOR;
m_call_table[0xEE] = &Processor::INC; m_call_table[0xFE] = &Processor::INC; m_call_table[0xE6] = &Processor::INC;  m_call_table[0xF6] = &Processor::INC;
m_call_table[0xE8] = &Processor::INX; m_call_table[0xC8] = &Processor::INY; m_call_table[0x4C] = &Processor::JMP;  m_call_table[0x6C] = &Processor::JMP;
m_call_table[0x20] = &Processor::JSR; m_call_table[0xA9] = &Processor::LDA; m_call_table[0xAD] = &Processor::LDA;  m_call_table[0xBD] = &Processor::LDA; 
m_call_table[0xB9] = &Processor::LDA; m_call_table[0xA5] = &Processor::LDA; m_call_table[0xB5] = &Processor::LDA;  m_call_table[0xA1] = &Processor::LDA; 
m_call_table[0xB1] = &Processor::LDA; m_call_table[0xA2] = &Processor::LDX; m_call_table[0xAE] = &Processor::LDX;  m_call_table[0xBE] = &Processor::LDX; 
m_call_table[0xA6] = &Processor::LDX; m_call_table[0xB6] = &Processor::LDX; m_call_table[0xA0] = &Processor::LDY;  m_call_table[0xAC] = &Processor::LDY; 
m_call_table[0xBC] = &Processor::LDY; m_call_table[0xA4] = &Processor::LDY; m_call_table[0xB4] = &Processor::LDY;  m_call_table[0x4A] = &Processor::LSR; 
m_call_table[0x4E] = &Processor::LSR; m_call_table[0x5E] = &Processor::LSR; m_call_table[0x46] = &Processor::LSR;  m_call_table[0x56] = &Processor::LSR;
m_call_table[0xEA] = &Processor::NOP; m_call_table[0x09] = &Processor::ORA; m_call_table[0x0D] = &Processor::ORA;  m_call_table[0x1D] = &Processor::ORA; 
m_call_table[0x19] = &Processor::ORA; m_call_table[0x05] = &Processor::ORA; m_call_table[0x15] = &Processor::ORA;  m_call_table[0x01] = &Processor::ORA; 
m_call_table[0x11] = &Processor::ORA; m_call_table[0x48] = &Processor::PHA; m_call_table[0x08] = &Processor::PHP;  m_call_table[0x68] = &Processor::PLA;
m_call_table[0x28] = &Processor::PLP; m_call_table[0x2A] = &Processor::ROL; m_call_table[0x2E] = &Processor::ROL;  m_call_table[0x3E] = &Processor::ROL; 
m_call_table[0x26] = &Processor::ROL; m_call_table[0x36] = &Processor::ROL; m_call_table[0x6A] = &Processor::ROR;  m_call_table[0x6E] = &Processor::ROR; 
m_call_table[0x7E] = &Processor::ROR; m_call_table[0x66] = &Processor::ROR; m_call_table[0x76] = &Processor::ROR;  m_call_table[0x40] = &Processor::RTI;
m_call_table[0x60] = &Processor::RTS; m_call_table[0xE9] = &Processor::SBC; m_call_table[0xED] = &Processor::SBC;  m_call_table[0xFD] = &Processor::SBC; 
m_call_table[0xF9] = &Processor::SBC; m_call_table[0xE5] = &Processor::SBC; m_call_table[0xF5] = &Processor::SBC;  m_call_table[0xE1] = &Processor::SBC; 
m_call_table[0xF1] = &Processor::SBC; m_call_table[0x38] = &Processor::SEC; m_call_table[0xF8] = &Processor::SED;  m_call_table[0x78] = &Processor::SEI;
m_call_table[0x8D] = &Processor::STA; m_call_table[0x9D] = &Processor::STA; m_call_table[0x99] = &Processor::STA;  m_call_table[0x85] = &Processor::STA;
m_call_table[0x95] = &Processor::STA; m_call_table[0x81] = &Processor::STA; m_call_table[0x91] = &Processor::STA;  m_call_table[0x8E] = &Processor::STX; 
m_call_table[0x86] = &Processor::STX; m_call_table[0x96] = &Processor::STX; m_call_table[0x8C] = &Processor::STY;  m_call_table[0x84] = &Processor::STY; 
m_call_table[0x94] = &Processor::STY; m_call_table[0xAA] = &Processor::TAX; m_call_table[0xA8] = &Processor::TAY;  m_call_table[0xBA] = &Processor::TSX;
m_call_table[0x8A] = &Processor::TXA; m_call_table[0x9A] = &Processor::TXS; m_call_table[0x98] = &Processor::TYA;
}


void Processor::execute_read_modify_write(std::function<void(uint8_t&)> modify){
    switch(m_addressing_mode){
        case ACCUMULATOR:
        { 
            modify(m_registers.AC);
            dummy_read(static_cast<uint16_t>(0x0));

        }
            break;
        case ZEROPAGE:
        {  
            uint8_t zero_page_adl = fetch();
            uint8_t data = m_bus.read(static_cast<uint16_t>(zero_page_adl));
            dummy_read(static_cast<uint16_t>(zero_page_adl));

            uint8_t data_to_modify = data;

            modify(data_to_modify);

            uint8_t modified_data = data_to_modify;

            // Write data back to memory
            m_bus.write(static_cast<uint16_t>(zero_page_adl), modified_data);
        }
            break;

        case ABSOLUTE:
        {
            uint8_t abs_addr_low_byte = fetch();
            uint8_t abs_addr_high_byte = fetch();
            uint16_t abs_address = (abs_addr_high_byte << 8) | abs_addr_low_byte;

            uint8_t data = m_bus.read(abs_address);

            dummy_read(abs_address);

            uint8_t data_to_modify = data;

            modify(data_to_modify);

            uint8_t modified_data = data_to_modify;

            m_bus.write(abs_address, modified_data);
        }
            break;
        case ZEROPAGEX:
        {
            uint8_t zero_page_base_addr = fetch();
            dummy_read(zero_page_base_addr);

            uint8_t data = m_bus.read(zero_page_base_addr + m_registers.IX);

            dummy_read(zero_page_base_addr + m_registers.IX);

            uint8_t data_to_modify = data;

            modify(data_to_modify);

            uint8_t modified_data = data_to_modify;

            m_bus.write(static_cast<uint16_t>(zero_page_base_addr + m_registers.IX) , modified_data);
        }

            break;
        case ABSOLUTEX:
        {
            uint8_t abs_addr_low_byte = fetch();
            uint8_t abs_addr_high_byte = fetch();
            uint16_t abs_address = (abs_addr_high_byte << 8) | abs_addr_low_byte;

            dummy_read(abs_address + m_registers.IX);

            if(abs_addr_low_byte + m_registers.IX > 255){
                    abs_addr_high_byte++;
            }

            uint8_t data = m_bus.read((abs_addr_high_byte << 8) | abs_addr_low_byte);

            uint8_t data_to_modify = data;

            modify(data_to_modify);

            uint8_t modified_data = data_to_modify;

            dummy_read(abs_address);

            m_bus.write((abs_addr_high_byte << 8) | abs_addr_low_byte, modified_data);
        }
            break;
        default:
            // The switch statement should never reach the default case
    }
}

void Processor::execute_store(std::function<void(uint16_t)> write) {
    switch(m_addressing_mode) {
        case ZEROPAGE:
            {
                uint8_t zero_page_addr = fetch();
                write(static_cast<uint16_t>(zero_page_addr));
            }
            break;
        case ABSOLUTE:
            {
                uint8_t abs_addr_low_byte = fetch();
                uint8_t abs_addr_high_byte = fetch();
                uint16_t abs_address = (abs_addr_high_byte << 8) | abs_addr_low_byte;
                write(abs_address);
            }
            break;
        case INDIRECTX:
            {
                uint8_t zero_page_base_addr = fetch();
                uint8_t indr_addr_low = zero_page_base_addr + m_registers.IX;
                uint8_t indr_addr_high = indr_addr_low + 1;
                uint16_t full_indr_addr = (indr_addr_high << 8) | indr_addr_low;
                write(full_indr_addr);
            }
            break;
        case ABSOLUTEX:
            {
                uint8_t abs_addr_low_byte = fetch();
                uint8_t abs_addr_high_byte = fetch();
                uint8_t adl = abs_addr_low_byte + m_registers.IX;
                uint8_t adh = abs_addr_high_byte;
                uint16_t abs_address = (adh << 8) | adl;
                dummy_read(abs_address);
                if(adl + m_registers.IX > 255){
                    adh++;
                }
                write((adh << 8) | adl);
            }
            break;
        case ABSOLUTEY:
            {
                uint8_t abs_addr_low_byte = fetch();
                uint8_t abs_addr_high_byte = fetch();
                uint8_t adl = abs_addr_low_byte + m_registers.IY;
                uint8_t adh = abs_addr_high_byte;
                uint16_t abs_address = (adh << 8) | adl;
                dummy_read(abs_address);
                if(adl + m_registers.IY > 255){
                    adh++;
                }
                write((adh << 8) | adl);
            }
            break;
        case ZEROPAGEX:
            {
                uint8_t zero_page_base_address = fetch();
                dummy_read(static_cast<uint16_t>(zero_page_base_address));
                uint8_t zero_page_address_x = zero_page_base_address + m_registers.IX;
                write(zero_page_address_x);
            }
            break;
        case ZEROPAGEY:
            {
                uint8_t zero_page_base_address = fetch();
                dummy_read(static_cast<uint16_t>(zero_page_base_address));
                uint8_t zero_page_address_y = zero_page_base_address + m_registers.IY;
                write(zero_page_address_y);
            }
            break;
        case INDIRECTY:
            {
                uint8_t zero_page_base_addr = fetch();
                uint8_t indr_addr_low = zero_page_base_addr + m_registers.IY;
                uint8_t indr_addr_high = indr_addr_low + 1;
                uint16_t full_indr_addr = (indr_addr_high << 8) | indr_addr_low;
                write(full_indr_addr);
            }
            break;
        default:
            // The switch statement should never reach the default case
            break;
    }
}

void Processor::execute_internal_operation(std::function<void(uint8_t)> internal_operation){
        switch(m_addressing_mode){
            case IMMEDIATE:
            {
                uint8_t data = fetch();
                internal_operation(data);
            }
                break;
            case ZEROPAGE:
            {
                uint8_t effective_address = fetch();
                uint8_t data = m_bus.read(static_cast<uint16_t>(effective_address));
                internal_operation(data);
            }
                break;
            case ABSOLUTE:
            {
                uint8_t ADL = static_cast<uint16_t>(fetch());
                uint8_t ADH = fetch();
                uint8_t data = m_bus.read((ADH << 8) | ADL);
                internal_operation(data);
            }
                break;
            case INDIRECTX:
            {
                uint8_t BAL = fetch();
                dummy_read(static_cast<uint16_t>(BAL));
                uint8_t ADL = m_bus.read(BAL + m_registers.IX);
                uint8_t ADH = m_bus.read(BAL + m_registers.IX + 1);
                uint8_t data = m_bus.read((static_cast<uint16_t>(ADH) << 8) | ADL);
                internal_operation(data);

            }
                break;
            case ABSOLUTEX:
            {
                uint8_t BAL = fetch();
                uint8_t BAH = fetch();

                //TODO: Check this code later on
                if(BAL + m_registers.IX > 255) {
                    dummy_read(static_cast<uint16_t>(BAH) << 8 | BAL + m_registers.IX);
                }

                uint8_t data = m_bus.read((static_cast<uint16_t>(BAH) << 8 | BAL) + m_registers.IX); 
                internal_operation(data);
            }
                break;
            case ABSOLUTEY:
            {
                uint8_t BAL = fetch();
                uint8_t BAH = fetch();

                if(BAL + m_registers.IX > 255) {
                    dummy_read(static_cast<uint16_t>(BAH) << 8 | BAL + m_registers.IY);
                }

                uint8_t data = m_bus.read((static_cast<uint16_t>(BAH) << 8 | BAL) + m_registers.IY); 
                internal_operation(data);
            }
                break;
             case ZEROPAGEX:
            {
                uint8_t BAL = fetch();
                dummy_read(static_cast<uint16_t>(BAL));
                uint8_t data = m_bus.read(static_cast<uint16_t>(BAL) + m_registers.IX);
                internal_operation(data);
            }
                break;
             case ZEROPAGEY:
            {
                uint8_t BAL = fetch();
                dummy_read(static_cast<uint16_t>(BAL));
                uint8_t data = m_bus.read(static_cast<uint16_t>(BAL) + m_registers.IY);
                internal_operation(data);
            }
                break;
            default:
                break;

        }
}