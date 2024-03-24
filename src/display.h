#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <stdint.h>

struct Display {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	uint32_t pixels[64 * 32];
};

struct Display * create_display();
void destroy_display(struct Display *display);
void render(struct Display *display);
void clear_display(struct Display *display);
int draw_sprite(struct Display *display, uint8_t *sprite, size_t x, size_t y, size_t n);

#endif
