#include "Bus.h"

Bus::Bus()
{
	// Connect CPU to communication bus
	cpu.ConnectBus(this);

	// Clear RAM, just in case
	for (auto& i : ram) i = 0xEA;
}

Bus::~Bus()
{

}

void Bus::write(uint16_t addr, uint8_t data)
{
	if (addr >= 0x000 && addr <= 0xFFFF)
	{
		ram[addr] = data;
	}
}

uint8_t Bus::read(uint16_t addr, bool bReadOnly)
{
	if (addr >= 0x000 && addr <= 0xFFFF)
	{
		return ram[addr];
	}
	return 0x00;
}