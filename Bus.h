#pragma once
#include <cstdint>
#include <array>
#include "olc6502.h"

class Bus
{
public:
    Bus();
    ~Bus();
public:
    // Devices connected to the emulated BUS (e.g. CPU, RAM)
    olc6502 cpu;
    std::array<uint8_t, 64 * 1024> ram;
public:
    //Bus Read and Write
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool bReadOnly = false);
};
