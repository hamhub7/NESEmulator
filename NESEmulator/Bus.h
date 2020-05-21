#pragma once
#include <cstdint>
#include <array>
#include "olc6502.h"
#include "olc2C02.h"
#include "Cartridge.h"

class Bus
{
public:
	Bus();
	~Bus();

public: // Devices on bus
	olc6502 cpu;

	// The 2C02 Picture Processing Unit
	olc2C02 ppu;

	// RAM
	std::array<uint8_t, 2048> cpuRam;

	// The cartridge or "GamePak"
	std::shared_ptr<Cartridge> cart;

public: // Bus read and write
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

public: // System interface
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void reset();
	void clock();

private:
	// A count of how many clocks have passed
	uint32_t nSystemClockCounter = 0;
};

