#include "app.h"
#include "cpu.h"
#include "display.h"
#include "rom.h"
#include <stdio.h>
#include <nfd.h>

int main(int argc, char *argv[]) {
	char *path = NULL;
	nfdresult_t result = NFD_OpenDialog(NULL, NULL, &path);
	
	if (result == NFD_OKAY) {
	} else if (result == NFD_CANCEL) {
		printf("No file selected\n");
		return 1;
	} else {
		fprintf(stderr, "Error: %s\n", NFD_GetError());
		return 1;
	}

	struct Display *display = create_display();
	if (display == NULL) {
		return 1;
	}

	struct CPU cpu = {
		.pc = 0x200,
		.sp = -1
	};

	struct App app = {
		.display = display,
		.cpu = &cpu
	};

	struct Rom *rom = load_rom(path);
	cpu_load_rom(app.cpu, rom);

	main_loop(&app);

	destroy_display(display);

	return 0;
}
