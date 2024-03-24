#include "app.h"
#include "display.h"
#include <stdio.h>

void main_loop(struct App *app) {
	SDL_Event event;
	int quit = 0;
	while (!quit) {
		execute_instruction(app);
		render(app->display);

		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
				case SDL_QUIT:
					quit = 1;
					break;
			}
		}
	}
}

void execute_instruction(struct App *app) {
	struct CPU *cpu = app->cpu;
	uint16_t opcode = (cpu->memory[cpu->pc] << 8) | cpu->memory[cpu->pc + 1];

	uint16_t nnn = opcode & 0x0fff;
	uint8_t n = opcode & 0x000f;
	uint8_t x = (opcode & 0x0f00) >> 8;
	uint8_t y = (opcode & 0x00f0) >> 4;
	uint8_t kk = opcode & 0x00ff;

	switch (opcode & 0xf000) {
		case 0x0000:
			switch (opcode & 0x00ff) {
				case 0x00e0:
					clear_display(app->display);
					cpu->pc += 2;
					break;
				case 0x00ee:
					cpu->pc = cpu->stack[cpu->sp];
					cpu->sp--;
					break;
			}
			break;
		case 0x1000:
			cpu->pc = nnn;
			break;
		case 0x2000:
			cpu->sp++;
			cpu->stack[cpu->sp] = cpu->pc;
			cpu->pc = nnn;
			break;
		case 0x3000:
			if (cpu->vx[x] == kk) {
				cpu->pc += 4;
			} else {
				cpu->pc += 2;
			}
			break;
		case 0x4000:
			if (cpu->vx[x] != kk) {
				cpu->pc += 4;
			} else {
				cpu->pc += 2;
			}
			break;
		case 0x5000:
			if (cpu->vx[x] == cpu->vx[y]) {
				cpu->pc += 4;
			} else {
				cpu->pc += 2;
			}
			break;
		case 0x6000:
			cpu->vx[x] = kk;
			cpu->pc += 2;
			break;
		case 0x7000:
			cpu->vx[x] += kk;
			cpu->pc += 2;
			break;
		case 0x8000:
			switch (opcode & 0x000f) {
				case 0x0000:
					cpu->vx[x] = cpu->vx[y];
					cpu->pc += 2;
					break;
				case 0x0001:
					cpu->vx[x] |= cpu->vx[y];
					cpu->pc += 2;
					break;
				case 0x0002:
					cpu->vx[x] &= cpu->vx[y];
					cpu->pc += 2;
					break;
				case 0x0003:
					cpu->vx[x] ^= cpu->vx[y];
					cpu->pc += 2;
					break;
				case 0x0004:
					if ((uint16_t)cpu->vx[x] + (uint16_t)cpu->vx[y] > 255) {
						cpu->vx[0xf] = 1;
					} else {
						cpu->vx[0xf] = 0;
					}
					cpu->vx[x] += cpu->vx[y];
					cpu->pc += 2;
					break;
				case 0x0005:
					if (cpu->vx[x] > cpu->vx[y]) {
						cpu->vx[0xf] = 1;
					} else {
						cpu->vx[0xf] = 0;
					}
					cpu->vx[x] -= cpu->vx[y];
					cpu->pc += 2;
					break;
				case 0x0006:
					cpu->vx[0xf] = cpu->vx[x] & 1;
					cpu->vx[x] = cpu->vx[y] >> 1;
					cpu->pc += 2;
					break;
				case 0x0007:
					if (cpu->vx[y] > cpu->vx[x]) {
						cpu->vx[0xf] = 1;
					} else {
						cpu->vx[0xf] = 0;
					}
					cpu->vx[x] = cpu->vx[y] - cpu->vx[x];
					cpu->pc += 2;
					break;
				case 0x000e:
					cpu->vx[0xf] = cpu->vx[x] >> 7;
					cpu->vx[x] = cpu->vx[y] << 1;
					cpu->pc += 2;
					break;
			}
			break;
		case 0x9000:
			if (cpu->vx[x] != cpu->vx[y]) {
				cpu->pc += 4;
			} else {
				cpu->pc += 2;
			}
			break;
		case 0xa000:
			cpu->i = nnn;
			cpu->pc += 2;
			break;
		case 0xd000:
			cpu->vx[0xf] = draw_sprite(app->display, &cpu->memory[cpu->i], cpu->vx[x], cpu->vx[y], n);
			cpu->pc += 2;
			break;
		default:
			fprintf(stderr, "Error: Unimplemented instruction: 0x%x\n", opcode);
			exit(1);
	}
}
