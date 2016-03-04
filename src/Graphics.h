#pragma once
#include <iostream>
#include <SDL2/SDL.h>

extern const char* TITLE="TPS2";
extern const int SCREEN_WIDTH= 800;
extern const int SCREEN_HEIGHT = 800;

namespace graphics{
	int create();
	void clear();
	void update();
	void close();
	SDL_Window* window=NULL;
	SDL_Surface* screenSurface=NULL;
}
