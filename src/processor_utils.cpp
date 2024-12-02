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
                uint8_t data = m_bus.read((ADH << 8) & ADL);
                internal_operation(data);
            }
                break;
            case INDIRECTX:
            {
                uint8_t BAL = fetch();
                dummy_read(static_cast<uint16_t>(BAL));
                uint8_t ADL = m_bus.read(BAL + m_registers.IX);
                uint8_t ADH = m_bus.read(BAL + m_registers.IX + 1);
                uint8_t data = m_bus.read((static_cast<uint16_t>(ADH) << 8) & ADL);
                internal_operation(data);

            }
                break;
            case ABSOLUTEX:
            {
                // uint8_t BAL = fetch();
                // uint8_t BAH = fetch();

            }
                break;
            case ABSOLUTEY:
            {

            }
                break;
             case ZEROPAGEX:
            {

            }
                break;
             case ZEROPAGEY:
            {

            }
                break;
            default:
                break;

        }
}