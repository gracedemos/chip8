#include "cpu.h"
#include "rom.h"
#include <string.h>

void cpu_load_rom(struct CPU *cpu, struct Rom *rom) {
	uint8_t *ptr = &cpu->memory[0x200];
	memcpy(ptr, rom->data, rom->size);

	destroy_rom(rom);
}
