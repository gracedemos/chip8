#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "rom.h"

struct CPU {
	uint8_t vx[16];
	uint16_t i;
	uint16_t pc;
	int8_t sp;
	uint16_t stack[16];
	uint8_t memory[0x1000];
};

void cpu_load_rom(struct CPU *cpu, struct Rom *rom);

#endif
