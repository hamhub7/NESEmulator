#include "Bus.h"

Bus::Bus()
{
	// Connect CPU to communication bus
	cpu.ConnectBus(this);

	// Clear RAM, just in case
	for (auto& i : cpuRam) i = 0x00;
}

Bus::~Bus()
{

}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
	if (cart->cpuWrite(addr, data))
	{
		// Cartridge Address Range
	}
	else if (addr >= 0x000 && addr <= 0x1FFF)
	{
		cpuRam[addr & 0x07FF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		ppu.cpuWrite(addr & 0x0007, data);
	}
	else if (addr >= 0x4016 && addr <= 0x4017)
	{
		controller_state[addr & 0x0001] = controller[addr & 0x0001];
	}
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
	uint8_t data = 0x00;

	if (cart->cpuRead(addr, data))
	{
		// Cartridge Address Range
	}
	else if (addr >= 0x000 && addr <= 0x1FFF)
	{
		data = cpuRam[addr & 0x07FF];
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		data = ppu.cpuRead(addr & 0x0007, bReadOnly);
	}
	else if (addr >= 0x4016 && addr <= 0x4017)
	{
		data = (controller_state[addr & 0x0001] & 0x80) > 0;
		controller_state[addr & 0x0001] <<= 1;
	}

	return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	this->cart = cartridge;
	ppu.ConnectCartridge(cartridge);
}

void Bus::reset()
{
	cpu.reset();
	nSystemClockCounter = 0;
}

void Bus::clock()
{
	ppu.clock();
	if (nSystemClockCounter % 3 == 0)
	{
		cpu.clock();
	}

	if (ppu.nmi)
	{
		ppu.nmi = false;
		cpu.nmi();
	}

	nSystemClockCounter++;
}
