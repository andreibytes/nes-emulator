#pragma once

#include "cartridge.hpp"
#include "common.hpp"

class Mapper {
    public:
      virtual uint8_t read(uint16_t address) = 0;
      virtual void write(uint8_t data, uint16_t address) = 0;
      virtual void reset() = 0;
};