#ifndef ROM_H
#define ROM_H

#include <stdint.h>
#include <stddef.h>

struct Rom {
	uint8_t *data;
	size_t size;
};

struct Rom * load_rom(const char *path);
void destroy_rom(struct Rom *rom);

#endif
