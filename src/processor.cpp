#include "processor.h"

#include <unordered_set>
#include <iostream>

Processor::Processor(Bus& bus): m_bus(bus) {
    setup_call_table();
    read_reset_vector();
}

void Processor::read_reset_vector() {
    uint16_t  reset_vector_address = 0;
    uint8_t   byte1 = m_bus.read(0xFFFC);
    uint8_t   byte2 = m_bus.read(0xFFFD);

    reset_vector_address &= byte1;
    reset_vector_address <<= 8;
    reset_vector_address &= byte2;

    m_registers.PC = reset_vector_address;
}

void Processor::execute_instruction() {
    uint8_t opcode = fetch_opcode();
    auto it = m_call_table.find(opcode);
    if(it != m_call_table.end()){
        (this->*(it->second))();
    }
}



uint8_t Processor::fetch() {
    uint8_t data = m_bus.read(m_registers.PC);
    m_registers.PC++;
    return data;
}

uint8_t Processor::fetch_opcode() {
    uint8_t data = m_bus.read(m_registers.PC);
    m_registers.PC++;
    return data;
}


/*
    All the 6502 instructions implemented down below
    Referenced documentation:
    https://web.archive.org/web/20221106105459if_/http://archive.6502.org/books/mcs6500_family_hardware_manual.pdf
    https://www.nesdev.org/obelisk-6502-guide/reference.html     
*/ 

void Processor::DEX() {

    m_registers.IX--;

    if(m_registers.IX == 0) {
        m_registers.S |= ZERO_FLAG;
    } else if (m_registers.IX & BIT7_MASK) {
        m_registers.S |= NEGATIVE_FLAG;
    }

    
}

void Processor::NOP() {
    
}


void Processor::TAX() {

    m_registers.IX = m_registers.AC;

    if(m_registers.IX == 0){
        m_registers.S |= ZERO_FLAG;
    } else if (m_registers.IX & BIT7_MASK) {
        m_registers.S |= NEGATIVE_FLAG;
    }

    
}

void Processor::TYA() {

    m_registers.AC = m_registers.IY;

    if(m_registers.AC == 0){
        m_registers.S |= ZERO_FLAG;
    } else if (m_registers.AC & BIT7_MASK) {
        m_registers.S |= NEGATIVE_FLAG;
    }

    
}

void Processor::CLC() {

    if(m_registers.S & CARRY_FLAG){
        m_registers.S ^= CARRY_FLAG;
    }

    
}


void Processor::DEY() {

    m_registers.IY--;

    if(m_registers.IY == 0){
        m_registers.S |= ZERO_FLAG;
    } else if (m_registers.IY & BIT7_MASK){
        m_registers.S |= NEGATIVE_FLAG;
    }

    
}

void Processor::TAY(){

    m_registers.IY = m_registers.AC;

     if(m_registers.IY == 0){
        m_registers.S |= ZERO_FLAG;
    } else if (m_registers.IY & BIT7_MASK){
        m_registers.S |= NEGATIVE_FLAG;
    }

    
}


void Processor::CLD() {

    if(m_registers.S & DECIMAL_FLAG){
        m_registers.S ^= DECIMAL_FLAG;
    }

    
}

void Processor::INX() {

    m_registers.IX++;

    if(m_registers.IX == 0){
        m_registers.S |= ZERO_FLAG;
    } else if (m_registers.IX & BIT7_MASK){
        m_registers.S |= NEGATIVE_FLAG;
    }

    
}

void Processor::SEC() {

    m_registers.S |= CARRY_FLAG;

    
}

void Processor::TSX() {
    
    m_registers.IX = m_registers.SP;

    if(m_registers.IX == 0) {
        m_registers.S |= ZERO_FLAG;
    } else if (m_registers.IX & BIT7_MASK) {
        m_registers.S |= NEGATIVE_FLAG;
    }

    
}

void Processor::CLI() {

    if(m_registers.S & INTERRUPT_DISABLE_FLAG) {
        m_registers.S ^= INTERRUPT_DISABLE_FLAG;
    }

    
}

void Processor::INY() {

    m_registers.IY++;

    if(m_registers.IY == 0){
        m_registers.S |= ZERO_FLAG;
    } else if (m_registers.IY & BIT7_MASK){
        m_registers.S |= NEGATIVE_FLAG;
    }

    
}


void Processor::SED() {

    m_registers.S |= DECIMAL_FLAG;

    
}

void Processor::TXA() {

    m_registers.AC = m_registers.IX;

     if(m_registers.AC == 0) {
        m_registers.S |= ZERO_FLAG;
    } else if (m_registers.AC & BIT7_MASK) {
        m_registers.S |= NEGATIVE_FLAG;
    }
    
    
}


void Processor::CLV() {

    if(m_registers.S & OVERFLOW_FLAG){
        m_registers.S ^= OVERFLOW_FLAG;
    }

    
}

void Processor::SEI() {

    m_registers.S |= INTERRUPT_DISABLE_FLAG;

    
}

void Processor::TXS() {

    m_registers.SP = m_registers.IX;

    
}


void Processor::BCC() {
    uint8_t branch_offset = fetch();

    if(!(m_registers.S & CARRY_FLAG)) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        
    }
}


void Processor::BCS() {
    uint8_t branch_offset = fetch();

    if(m_registers.S & CARRY_FLAG) {
         uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        
    }
}


void Processor::BEQ() {
    uint8_t branch_offset = fetch();

    if(m_registers.S & ZERO_FLAG) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        
    }
}


void Processor::BMI() {
    uint8_t branch_offset = fetch();

    if(m_registers.S & NEGATIVE_FLAG) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        
    }
}


void Processor::BNE() {
    uint8_t branch_offset = fetch();

    if(!(m_registers.S & ZERO_FLAG)) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        
    }
}

void Processor::BPL() {
    uint8_t branch_offset = fetch();

    if(!(m_registers.S & NEGATIVE_FLAG)) {
         uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        
    }
}


void Processor::BVC() {
    uint8_t branch_offset = fetch();

    if(!(m_registers.S & OVERFLOW_FLAG)) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        
    }
}

void Processor::BVS() {
    uint8_t branch_offset = fetch();

    if(m_registers.S & OVERFLOW_FLAG) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        
    }
}


void Processor::STA() {
    execute_store([this](uint16_t addr) {
        m_bus.write(addr, m_registers.AC);
    });
    
}


void Processor::STX() {
    execute_store([this](uint16_t addr) {
        m_bus.write(addr, m_registers.IX);
    });
    
}

void Processor::STY() {
    execute_store([this](uint16_t addr) {
        m_bus.write(addr, m_registers.IY);
    });
    
}


void Processor::ASL() {
    execute_read_modify_write([this] (uint8_t& ref) {
        if(ref& BIT7_MASK){
            m_registers.S |= CARRY_FLAG;
        }

        ref<<= 1;

        if(ref== 0){
            m_registers.S |= ZERO_FLAG;
        } else if (ref& BIT7_MASK){
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::DEC() {
    execute_read_modify_write([this] (uint8_t& ref) {
        ref--;
        if(ref == 0){
            m_registers.S |= ZERO_FLAG;
        }  else if (ref & BIT7_MASK){
            m_registers.S |= NEGATIVE_FLAG;
        }
    });
    
    
}

void Processor::INC() {
    execute_read_modify_write([this] (uint8_t& ref) {
        ref++;
        if(ref == 0){
            m_registers.S |= ZERO_FLAG;
        }  else if (ref & BIT7_MASK){
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}


void Processor::LSR() {
    execute_read_modify_write([this] (uint8_t& ref) {
        if(ref & BIT0_MASK){
            m_registers.S |= CARRY_FLAG;
        }

        ref >>= 1;

        if(ref == 0){
            m_registers.S |= ZERO_FLAG;
        } else if (ref & BIT7_MASK){
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::ROL() {
    execute_read_modify_write([this] (uint8_t& ref) {
        bool current_carry_flag_val = m_registers.S & CARRY_FLAG;

        if(ref & BIT7_MASK){
            m_registers.S |= CARRY_FLAG;
        }

        ref <<= 1;

        if(current_carry_flag_val) {
            ref |= BIT0_MASK;
        }

        if(ref == 0 && m_addressing_mode == ACCUMULATOR){
            m_registers.S |= ZERO_FLAG;
        } else if (ref & BIT7_MASK){
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::ROR() {
    execute_read_modify_write([this] (uint8_t& ref) {
       bool current_carry_flag_val = m_registers.S & CARRY_FLAG;

        if(ref & BIT0_MASK){
            m_registers.S |= CARRY_FLAG;
        }

        ref >>= 1;

        if(current_carry_flag_val) {
            ref |= BIT7_MASK;
        }

        if(ref == 0 && m_addressing_mode == ACCUMULATOR){
            m_registers.S |= ZERO_FLAG;
        } else if (ref & BIT7_MASK){
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::ADC() {
    execute_internal_operation([this] (uint8_t data){
            int carry = (m_registers.S & CARRY_FLAG) ? 1 : 0;
            uint16_t sum = data + m_registers.AC + carry;


            if(sum > 255) {
                // Carry
                m_registers.S |= CARRY_FLAG;
            }

            int16_t overflow_check = static_cast<int8_t>(data) + static_cast<int8_t>(m_registers.AC) + carry;
            if(overflow_check <= -128 || overflow_check >= 127) {
                m_registers.S |= OVERFLOW_FLAG;
            }

            m_registers.AC += data + carry;

            if(m_registers.AC == 0){
                m_registers.S |= ZERO_FLAG;
            } else if (m_registers.AC & BIT7_MASK){
                m_registers.S |= NEGATIVE_FLAG;
            }
    });

    
}

void Processor::AND() {
    execute_internal_operation([this] (uint8_t data) {
            m_registers.AC &= data;
            if(m_registers.AC == 0) {
                m_registers.S |= ZERO_FLAG;
            } else if (m_registers.AC & BIT7_MASK) {
                m_registers.S |= NEGATIVE_FLAG;
            }
    });

    
}

void Processor::BIT() {
    execute_internal_operation([this] (uint8_t data) {
        uint8_t bit_test_result = data & m_registers.AC;

        if(bit_test_result & BIT7_MASK){
            m_registers.S |= NEGATIVE_FLAG;
        } else if (bit_test_result & BIT6_MASK) {
            m_registers.S |= OVERFLOW_FLAG;
        }
    });

    
}

void Processor::CMP() {
    execute_internal_operation([this] (uint8_t data) {
        if(m_registers.AC >= data) {
            m_registers.S |= CARRY_FLAG;
        } else if (m_registers.AC == data) {
            m_registers.S |= ZERO_FLAG;
        } else if (data & BIT7_MASK) {
            m_registers.S |= NEGATIVE_FLAG;
        }
    });
    
    
}

void Processor::CPX() {
    execute_internal_operation([this] (uint8_t data) {
        if(m_registers.IX >= data) {
            m_registers.S |= CARRY_FLAG;
        } else if (m_registers.IX == data) {
            m_registers.S |= ZERO_FLAG;
        } else if (data & BIT7_MASK) {
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::CPY() {
    execute_internal_operation([this] (uint8_t data) {
        if(m_registers.IY >= data) {
            m_registers.S |= CARRY_FLAG;
        } else if (m_registers.IY == data) {
            m_registers.S |= ZERO_FLAG;
        } else if (data & BIT7_MASK) {
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::EOR() {
    execute_internal_operation([this] (uint8_t data) {
        m_registers.AC ^= data;
        if(m_registers.AC == 0) {
            m_registers.S |= ZERO_FLAG;
        } else if (m_registers.AC & BIT7_MASK) {
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::LDA() {
    execute_internal_operation([this] (uint8_t data) {
        m_registers.AC = data;

        if(m_registers.AC == 0) {
            m_registers.S |= ZERO_FLAG;
        } else if (m_registers.AC & BIT7_MASK){
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::LDX() {
    execute_internal_operation([this] (uint8_t data) {
        m_registers.IX = data;

        if(m_registers.IX == 0) {
            m_registers.S |= ZERO_FLAG;
        } else if (m_registers.IX & BIT7_MASK){
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::LDY() {
    execute_internal_operation([this] (uint8_t data) {
        m_registers.IY = data;

        if(m_registers.IY == 0) {
            m_registers.S |= ZERO_FLAG;
        } else if (m_registers.IY & BIT7_MASK){
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::ORA() {
    execute_internal_operation([this] (uint8_t data) {
        m_registers.AC |= data;
        if(m_registers.AC == 0) {
            m_registers.S |= ZERO_FLAG;
        } else if (m_registers.AC & BIT7_MASK) {
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::SBC() {
    execute_internal_operation([this] (uint8_t data){
        int carry = (m_registers.S & CARRY_FLAG) ? 1 : 0;
        uint16_t sum = ~data + m_registers.AC + carry;


        if(sum > 255) {
            // Carry
            m_registers.S |= CARRY_FLAG;
        }

        int16_t overflow_check = static_cast<int8_t>(~data) + static_cast<int8_t>(m_registers.AC) + carry;
        if(overflow_check <= -128 || overflow_check >= 127) {
            m_registers.S |= OVERFLOW_FLAG;
        }

        m_registers.AC += ~data + carry;

        if(m_registers.AC == 0){
            m_registers.S |= ZERO_FLAG;
        } else if (m_registers.AC & BIT7_MASK){
            m_registers.S |= NEGATIVE_FLAG;
        }
    });

    
}

void Processor::PHP() {
    
    m_bus.write(m_registers.SP, m_registers.S);
}

void Processor::PHA() {
    
    m_bus.write(m_registers.SP, m_registers.AC);
}

void Processor::PLP() {
    
    dummy_read(m_registers.SP);
    uint8_t data = m_bus.read(m_registers.SP + 1);
    m_registers.S = data;
}


void Processor::PLA() {
    
    dummy_read(m_registers.SP);
    uint8_t data = m_bus.read(m_registers.SP + 1);
    m_registers.AC = data;
}


void Processor::JSR() {
    uint8_t ADL = fetch();
    dummy_read(m_registers.SP);
    uint8_t PCH = static_cast<uint8_t>(m_registers.PC >> 8);
    uint8_t PCL = static_cast<uint8_t>((m_registers.PC << 8) >> 8);
    m_bus.write(m_registers.SP, PCH);
    m_bus.write(m_registers.SP - 1, PCL);
    uint8_t ADH = fetch();
    m_registers.PC = (static_cast<uint16_t>(ADH) << 8) & static_cast<uint16_t>(ADL) - 1;
    
}

void Processor::BRK() {
    dummy_read(m_registers.PC + 1);
    uint8_t PCH = static_cast<uint8_t>(m_registers.PC >> 8);
    uint8_t PCL = static_cast<uint8_t>((m_registers.PC << 8) >> 8);
    m_bus.write(m_registers.SP, PCH);
    m_bus.write(m_registers.SP - 1, PCL);
    m_bus.write(m_registers.SP - 2, m_registers.S);
    uint8_t ADL = fetch();
    uint8_t ADH = fetch();
    m_registers.PC = (static_cast<uint16_t>(ADH) << 8) & static_cast<uint16_t>(ADL) - 1;
    

}

void Processor::RTI() {
    dummy_read(m_registers.PC + 1);
    dummy_read(m_registers.SP);
    uint8_t status_register = m_bus.read(m_registers.SP + 1);
    uint8_t PCL = m_bus.read(m_registers.SP + 2);
    uint8_t PCH = m_bus.read(m_registers.SP + 3);
    m_registers.S = status_register;
    m_registers.PC = (static_cast<uint16_t>(PCH) << 8) & static_cast<uint16_t>(PCL) - 1;
    
}

void Processor::JMP() {
    switch (m_addressing_mode)
    {
    case ABSOLUTE:
        uint8_t ADL = fetch();
        uint8_t ADH = fetch();
        m_registers.PC = (static_cast<uint16_t>(ADH) << 8) & static_cast<uint16_t>(ADL) - 1;
        
        break;
    case INDIRECT:
        std::cout << "Indirect addressing for JMP not implemented yet" << std::endl;
        // break;
        // uint8_t IAL = fetch();
        // uint8_t IAH = fetch();
        // uint16_t indirect_address = (static_cast<uint16_t>(IAH) << 8) & static_cast<uint16_t>(IAL);

    default:
        break;
    }
}

void Processor::RTS() {
    dummy_read(m_registers.PC + 1);
    dummy_read(m_registers.SP);
    uint8_t PCL = m_bus.read(m_registers.SP + 1);
    uint8_t PCH = m_bus.read(m_registers.SP + 2);
    m_registers.PC = (static_cast<uint16_t>(PCH) << 8) & static_cast<uint16_t>(PCL);
    dummy_read(m_registers.PC);
    

}