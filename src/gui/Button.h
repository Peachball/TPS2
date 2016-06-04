#pragma once
#include "GUIComponent.h"
#include <SDL2/SDL.h>
#include "Graphics.h"
#include <vector>

using namespace graphics;

class Button: public GUIComponent{
	public:
		Button(std::string filepath, int xpos = 0, int ypos = 0,
				double xscale=1, double yscale=1);
		~Button();
		bool display();
		void setListener(void (*function)(SDL_Event*));
		void processEvent(const SDL_Event* event);
	private:
		SDL_Texture* image;
		std::vector<void (*)(SDL_Event*)> callbacks;
		SDL_Rect dstRect;
};
