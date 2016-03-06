#pragma once
#include <SDL2/SDL.h>

class GUIComponent{
	public:
		virtual bool display()=0;
		virtual void processEvent(const SDL_Event* event)=0;
};
