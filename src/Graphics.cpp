#include "Graphics.h"

namespace graphics{

	SDL_Window* window=NULL;
	SDL_Surface* screenSurface=NULL;
	int create(){


		if(SDL_Init(SDL_INIT_VIDEO) < 0){
			std::cout<<"Unable to instantiate SDL: "<<SDL_GetError()<<'\n';
			return -1;
		}

		int flags = IMG_INIT_JPG|IMG_INIT_PNG;
		if( (IMG_Init(flags) & flags) != flags){
			std::cout<<"Failed to load IMG correctly\n";
			std::cout<<"Error: "<<IMG_GetError()<<'\n';
		}

		window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
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
		window = NULL;
		SDL_Quit();
	}

	void closeSurface(SDL_Surface* surface){
		SDL_FreeSurface(surface);
		surface = NULL;
	}

	SDL_Surface* loadImage(const char* source){
		SDL_Surface* target, *optimized = NULL;
		std::string buffer(RESOURCE_DIR);
		buffer.append(source);
		target = IMG_Load(buffer.c_str());
		if(target == NULL){
			std::cout<<"Unable to load image:"<<source<<'\n';
			std::cout<<"Error: "<<SDL_GetError()<<'\n';
		}
		else{
			optimized = SDL_ConvertSurface(target, screenSurface->format, NULL);
			closeSurface(target);
			if(optimized == NULL){
				std::cout<<"Failed to convert surface. Error: "<<SDL_GetError()<<'\n';
			}
		}
		return optimized;
	}
}
