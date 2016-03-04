#include "Graphics.h"

namespace graphics{

	int create(){

		if(SDL_Init(SDL_INIT_VIDEO) < 0){
			std::cout<<"Unable to instantiate SDL: "<<SDL_GetError()<<'\n';
			return -1;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
				SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if(window == NULL){
			std::cout<<"Unable to instantiate window: "<<SDL_GetError()<<'\n';
			return -1;
		}
		screenSurface = SDL_GetWindowSurface(window);
	}

	void clear(){
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
	}

	void update(){
		SDL_UpdateWindowSurface(window);
	}

	void close(){
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}
