#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static const char* const RESOURCE_DIR="res/";
static const char* const WINDOW_TITLE="TPS2";
static const int SCREEN_WIDTH= 1024;
static const int SCREEN_HEIGHT = 768;

void logError(std::string s);

namespace graphics{
	int create();
	void clear();
	void update();
	void closeSDL();
	SDL_Surface* loadImage(const char* source);
	SDL_Texture* loadTexture(const char* source);
	void close(SDL_Surface* surface);
	void close(SDL_Texture* texture);

	extern SDL_Window* window;
	extern SDL_Surface* screenSurface;
	extern SDL_Renderer* render;

	void logError();
}

#endif
