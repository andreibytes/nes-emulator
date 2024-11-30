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


uint8_t Processor::fetch() {
    uint8_t data = m_bus.read(m_registers.PC);
    m_registers.PC++;
    return data;
}

void Processor::fetch_opcode() {
    uint8_t data = m_bus.read(m_registers.PC);
    m_registers.PC++;
}

/*
    For single byte instructions on the second clock cycle the next opcode is fetched
    but discarded so that's why I call fetch and do nothing with the result.
*/

void Processor::DEX() {

    m_registers.IX--;

    if(m_registers.IX == 0) {
        m_registers.S |= ZERO_MASK;
    } else if (m_registers.IX & BIT7_MASK) {
        m_registers.S |= NEGATIVE_MASK;
    }

    fetch_opcode();
}

void Processor::NOP() {
    fetch_opcode();
}


void Processor::TAX() {

    m_registers.IX = m_registers.AC;

    if(m_registers.IX == 0){
        m_registers.S |= ZERO_MASK;
    } else if (m_registers.IX & BIT7_MASK) {
        m_registers.S |= NEGATIVE_MASK;
    }

    fetch_opcode();
}

void Processor::TYA() {

    m_registers.AC = m_registers.IY;

    if(m_registers.AC == 0){
        m_registers.S |= ZERO_MASK;
    } else if (m_registers.AC & BIT7_MASK) {
        m_registers.S |= NEGATIVE_MASK;
    }

    fetch_opcode();
}

void Processor::CLC() {

    if(m_registers.S & CARRY_MASK){
        m_registers.S ^= CARRY_MASK;
    }

    fetch_opcode();
}


void Processor::DEY() {

    m_registers.IY--;

    if(m_registers.IY == 0){
        m_registers.S |= ZERO_MASK;
    } else if (m_registers.IY & BIT7_MASK){
        m_registers.S |= NEGATIVE_MASK;
    }

    fetch_opcode();
}

void Processor::TAY(){

    m_registers.IY = m_registers.AC;

     if(m_registers.IY == 0){
        m_registers.S |= ZERO_MASK;
    } else if (m_registers.IY & BIT7_MASK){
        m_registers.S |= NEGATIVE_MASK;
    }

    fetch_opcode();
}


void Processor::CLD() {

    if(m_registers.S & DECIMAL_MASK){
        m_registers.S ^= DECIMAL_MASK;
    }

    fetch_opcode();
}

void Processor::INX() {

    m_registers.IX++;

    if(m_registers.IX == 0){
        m_registers.S |= ZERO_MASK;
    } else if (m_registers.IX & BIT7_MASK){
        m_registers.S |= NEGATIVE_MASK;
    }

    fetch_opcode();
}

void Processor::SEC() {

    m_registers.S |= CARRY_MASK;

    fetch_opcode();
}

void Processor::TSX() {
    
    m_registers.IX = m_registers.SP;

    if(m_registers.IX == 0) {
        m_registers.S |= ZERO_MASK;
    } else if (m_registers.IX & BIT7_MASK) {
        m_registers.S |= NEGATIVE_MASK;
    }

    fetch_opcode();
}

void Processor::CLI() {

    if(m_registers.S & INTERRUPT_DISABLE_MASK) {
        m_registers.S ^= INTERRUPT_DISABLE_MASK;
    }

    fetch_opcode();
}

void Processor::INY() {

    m_registers.IY++;

    if(m_registers.IY == 0){
        m_registers.S |= ZERO_MASK;
    } else if (m_registers.IY & BIT7_MASK){
        m_registers.S |= NEGATIVE_MASK;
    }

    fetch_opcode();
}


void Processor::SED() {

    m_registers.S |= DECIMAL_MASK;

    fetch_opcode();
}

void Processor::TXA() {

    m_registers.AC = m_registers.IX;

     if(m_registers.AC == 0) {
        m_registers.S |= ZERO_MASK;
    } else if (m_registers.AC & BIT7_MASK) {
        m_registers.S |= NEGATIVE_MASK;
    }
    
    fetch_opcode();
}


void Processor::CLV() {

    if(m_registers.S & OVERFLOW_MASK){
        m_registers.S ^= OVERFLOW_MASK;
    }

    fetch_opcode();
}

void Processor::SEI() {

    m_registers.S |= INTERRUPT_DISABLE_MASK;

    fetch_opcode();
}

void Processor::TXS() {

    m_registers.SP = m_registers.IX;

    fetch_opcode();
}


void Processor::BCC() {
    uint8_t branch_offset = fetch();

    if(!(m_registers.S & CARRY_MASK)) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        fetch_opcode();
    }
}


void Processor::BCS() {
    uint8_t branch_offset = fetch();

    if(m_registers.S & CARRY_MASK) {
         uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        fetch_opcode();
    }
}


void Processor::BEQ() {
    uint8_t branch_offset = fetch();

    if(m_registers.S & ZERO_MASK) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        fetch_opcode();
    }
}


void Processor::BMI() {
    uint8_t branch_offset = fetch();

    if(m_registers.S & NEGATIVE_MASK) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        fetch_opcode();
    }
}


void Processor::BNE() {
    uint8_t branch_offset = fetch();

    if(!(m_registers.S & ZERO_MASK)) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        fetch_opcode();
    }
}

void Processor::BPL() {
    uint8_t branch_offset = fetch();

    if(!(m_registers.S & NEGATIVE_MASK)) {
         uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        fetch_opcode();
    }
}


void Processor::BVC() {
    uint8_t branch_offset = fetch();

    if(!(m_registers.S & OVERFLOW_MASK)) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        fetch_opcode();
    }
}

void Processor::BVS() {
    uint8_t branch_offset = fetch();

    if(m_registers.S & OVERFLOW_MASK) {
        uint8_t dummy_read = m_bus.read(m_registers.PC);
        uint16_t updated_pc = m_registers.PC + branch_offset;


        // Check if page boundary crossed
        if((m_registers.PC % 256) + branch_offset > 256) {
            uint8_t dummy_page_read = m_bus.read((m_registers.PC & 0xFF00) + (updated_pc & 0x00FF));
        }

            
        m_registers.PC = updated_pc;
    } else {
        fetch_opcode();
    }
}


void Processor::STA() {
    execute_store([this](uint16_t addr) {
        m_bus.write(addr, m_registers.AC);
    });
    fetch_opcode();
}


void Processor::STX() {
    execute_store([this](uint16_t addr) {
        m_bus.write(addr, m_registers.IX);
    });
    fetch_opcode();
}

void Processor::STY() {
    execute_store([this](uint16_t addr) {
        m_bus.write(addr, m_registers.IY);
    });
    fetch_opcode();
}


void Processor::ASL() {
    execute_read_modify_write([this] (uint8_t& ref) {
        if(ref& BIT7_MASK){
            m_registers.S |= CARRY_MASK;
        }

        ref<<= 1;

        if(ref== 0){
            m_registers.S |= ZERO_MASK;
        } else if (ref& BIT7_MASK){
            m_registers.S |= NEGATIVE_MASK;
        }
    });
}

void Processor::DEC() {
    execute_read_modify_write([this] (uint8_t& ref) {
        ref--;
        if(ref == 0){
            m_registers.S |= ZERO_MASK;
        }  else if (ref & BIT7_MASK){
            m_registers.S |= NEGATIVE_MASK;
        }
    });
}

void Processor::INC() {
    execute_read_modify_write([this] (uint8_t& ref) {
        ref++;
        if(ref == 0){
            m_registers.S |= ZERO_MASK;
        }  else if (ref & BIT7_MASK){
            m_registers.S |= NEGATIVE_MASK;
        }
    });
}


void Processor::LSR() {
    execute_read_modify_write([this] (uint8_t& ref) {
        if(ref & BIT0_MASK){
            m_registers.S |= CARRY_MASK;
        }

        ref >>= 1;

        if(ref == 0){
            m_registers.S |= ZERO_MASK;
        } else if (ref & BIT7_MASK){
            m_registers.S |= NEGATIVE_MASK;
        }
    });
}

void Processor::ROL() {
    execute_read_modify_write([this] (uint8_t& ref) {
        bool current_carry_flag_val = m_registers.S & CARRY_MASK;

        if(ref & BIT7_MASK){
            m_registers.S |= CARRY_MASK;
        }

        ref <<= 1;

        if(current_carry_flag_val) {
            ref |= BIT0_MASK;
        }

        if(ref == 0 && m_addressing_mode == ACCUMULATOR){
            m_registers.S |= ZERO_MASK;
        } else if (ref & BIT7_MASK){
            m_registers.S |= NEGATIVE_MASK;
        }
    });
}

void Processor::ROR() {
    execute_read_modify_write([this] (uint8_t& ref) {
       bool current_carry_flag_val = m_registers.S & CARRY_MASK;

        if(ref & BIT0_MASK){
            m_registers.S |= CARRY_MASK;
        }

        ref >>= 1;

        if(current_carry_flag_val) {
            ref |= BIT7_MASK;
        }

        if(ref == 0 && m_addressing_mode == ACCUMULATOR){
            m_registers.S |= ZERO_MASK;
        } else if (ref & BIT7_MASK){
            m_registers.S |= NEGATIVE_MASK;
        }
    });
}