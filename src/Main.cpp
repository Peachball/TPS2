#include <SDL2/SDL.h>
#include <iostream>
#include "Graphics.h"

int main(){
	graphics::create();

	graphics::clear();

	SDL_Surface* z = graphics::loadImage("bigcircle.png");

	SDL_BlitSurface(z, NULL, graphics::screenSurface, NULL);

	graphics::update();
	
	SDL_Delay(2000);

	graphics::close();

	return 0;
}
