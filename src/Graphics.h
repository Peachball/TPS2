#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const char* const RESOURCE_DIR="res/";
const char* const WINDOW_TITLE="TPS2";
const int SCREEN_WIDTH= 800;
const int SCREEN_HEIGHT = 800;

namespace graphics{
	int create();
	void clear();
	void update();
	void close();
	SDL_Surface* loadImage(const char* source);
	SDL_Texture* loadTexture(const char* source);
	void closeSurface(SDL_Surface* surface);
	void closeTexture(SDL_Texture* texture);

	extern SDL_Window* window;
	extern SDL_Surface* screenSurface;
	extern SDL_Renderer* render;
}

#endif
