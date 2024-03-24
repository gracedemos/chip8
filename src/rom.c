#include "rom.h"
#include <stdio.h>
#include <stdlib.h>

struct Rom * load_rom(const char *path) {
	FILE *file = fopen(path, "r");
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);

	uint8_t *buffer = malloc(sizeof(uint8_t) * size);
	fread(buffer, size, 1, file);
	fclose(file);

	struct Rom *rom = malloc(sizeof(struct Rom));
	rom->data = buffer;
	rom->size = size;

	return rom;
}

void destroy_rom(struct Rom *rom) {
	free(rom->data);
	free(rom);
}
