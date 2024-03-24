#include "display.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Display * create_display() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_Window *window = SDL_CreateWindow(
		"Chip8",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if (window == NULL) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_Texture *texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STATIC,
		64,
		32
	);

	struct Display *display = malloc(sizeof(struct Display));
	display->window = window;
	display->renderer = renderer;
	display->texture = texture;

	memset(display->pixels, 0, sizeof(uint32_t) * 64 * 32);

	return display;
}

void destroy_display(struct Display *display) {
	SDL_DestroyTexture(display->texture);
	SDL_DestroyRenderer(display->renderer);
	SDL_DestroyWindow(display->window);
	SDL_Quit();

	free(display);
}

void render(struct Display *display) {
	SDL_UpdateTexture(display->texture, NULL, display->pixels, 64 * sizeof(uint32_t));
	SDL_RenderCopy(display->renderer, display->texture, NULL, NULL);
	SDL_RenderPresent(display->renderer);
}

void clear_display(struct Display *display) {
	memset(display->pixels, 0, sizeof(uint32_t) * 64 * 32);
}

int draw_sprite(struct Display *display, uint8_t *sprite, size_t x, size_t y, size_t n) {
	size_t index = y * 64 + x;
	int ret = 0;

	for (size_t i = 0; i < n; i++) {
		int bit_shift = 7;
		while (bit_shift >= 0) {
			uint8_t sprite_pixel = (sprite[i] >> bit_shift) & 1;
			if (sprite_pixel == 1 && display->pixels[i] == 0xff8000ff) {
				ret = 1;
			}

			if (sprite_pixel == 1) {
				display->pixels[index] ^= 0xff8000ff;
			}

			index++;
			bit_shift--;
		}
		index -= 8;
		index += 64;
	}

	return ret;
}
