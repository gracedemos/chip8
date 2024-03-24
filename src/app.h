#ifndef APP_H
#define APP_H

#include "display.h"
#include "cpu.h"

struct App {
	struct Display *display;
	struct CPU *cpu;
};

void main_loop(struct App *app);
void execute_instruction(struct App *app);

#endif
