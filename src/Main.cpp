#include <SDL2/SDL.h>
#include <iostream>
#include "Graphics.h"

int main(){
	std::cout<<"Hello world\n";

	graphics::create();

	graphics::clear();

	graphics::update();
	
	SDL_Delay(2000);

	graphics::close();

	return 0;
}
