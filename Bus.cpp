#include "Bus.h"

Bus::Bus(/* args */)
{
    // Clear cpuRAM
    for (auto &i : cpuRAM) i = 0x00;

    // Connect CPU, allowing CPU to write to RAM
    cpu.ConnectBus(this);
}

Bus::~Bus()
{
}

void Bus::write(uint16_t addr, uint8_t data)
{
    if (addr >= 0x0000 && addr <= 0xFFFF)
        cpuRAM[addr] = data;
}

uint8_t Bus::read(uint16_t addr, bool bReadOnly)
{   
    uint8_t data = 0x00;

    if (addr >= 0x0000 && addr <= 0xFFFF)
    {
        data = cpuRAM[addr & 0x07FF];
    }

    return 0x00;
}