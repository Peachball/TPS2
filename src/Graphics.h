#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <iostream>
#include <SDL2/SDL.h>

const char* const WINDOW_TITLE="TPS2";
const int SCREEN_WIDTH= 800;
const int SCREEN_HEIGHT = 800;

namespace graphics{
	int create();
	void clear();
	void update();
	void close();
	extern SDL_Window* window;
	extern SDL_Surface* screenSurface;
}

#endif
