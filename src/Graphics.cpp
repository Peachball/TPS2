#include "Graphics.h"

namespace graphics{

	SDL_Window* window         = NULL;
	SDL_Surface* screenSurface = NULL;
	SDL_Renderer* render       = NULL;

	//Status: whether or not the sdl stuff is ready for drawing
	bool status = false;

	int create(){
		status = true;

		if(SDL_Init(SDL_INIT_VIDEO) < 0){
			std::cout<<"Unable to instantiate SDL: "<<SDL_GetError()<<'\n';
			status = false;
			return -1;
		}

		int flags = IMG_INIT_JPG|IMG_INIT_PNG;
		if((IMG_Init(flags) & flags) != flags){
			std::cout<<"Failed to load IMG correctly\n";
			std::cout<<"Error: "<<IMG_GetError()<<'\n';
			status = false;
		}

		window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
				SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if(window == NULL){
			std::cout<<"Unable to instantiate window: "<<SDL_GetError()<<'\n';
			status = false;
			return -1;
		}
		screenSurface = SDL_GetWindowSurface(window);

		render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
		if(render == NULL){
			std::cout<<"Failed to load sdl renderer\n";
			std::cout<<"Error: "<<SDL_GetError()<<'\n';
			status = false;
		}

		return !status;
	}

	void clear(){
		if(!status){
			std::cout<<"SDL not fully initialized...\n";
			return;
		}
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
	}

	void update(){
		if(!status){
			std::cout<<"SDL not fully initialized...\n";
			return;
		}

		SDL_UpdateWindowSurface(window);
	}

	void closeSDL(){
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(render);
		window = NULL;

		SDL_Quit();
	}

	void close(SDL_Surface* surface){
		SDL_FreeSurface(surface);
		surface = NULL;
	}

	void close(SDL_Texture* texture){
		SDL_DestroyTexture(texture);
		texture = NULL;
	}

	SDL_Surface* loadImage(const char* source){
		if(!status){
			std::cout<<"SDL not fully initialized...\n";
			return NULL;
		}
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
			close(target);
			if(optimized == NULL){
				std::cout<<"Failed to convert surface. Error: "<<SDL_GetError()<<'\n';
			}
		}
		return optimized;
	}
	
	SDL_Texture* loadTexture(const char* source){
		if(!status){
			std::cout<<"SDL not fully initialized...\n";
			return NULL;
		}
		SDL_Surface* optimizedSurface = loadImage(source);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(render, optimizedSurface);
		close(optimizedSurface);

		if(texture == NULL){
			std::cout<<"Unable to load texture\n";
			std::cout<<"Error: "<<SDL_GetError()<<'\n';
		}
		return texture;
	}

	void logError(){
		std::cout<<"SDL Error: "<< SDL_GetError()<<'\n';
	}

}
