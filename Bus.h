#pragma once
#include <cstdint>
#include <array>
#include "olc6502.h"
#include "PPU.h"

class Bus
{
public:
    Bus();
    ~Bus();
public:
    // Devices connected to the emulated BUS (e.g. CPU, PPU, RAM)
    olc6502 cpu;
    PPU ppu;
    std::array<uint8_t, 64 * 1024> cpuRAM;

public:
    //Bus Read and Write
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool bReadOnly = false);
};
