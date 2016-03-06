#pragma once
#include <thread>
#include "Graphics.h"

class GUIHandler{
	public:
		void start();
		void stop();
		~GUIHandler();

	private:
		SDL_Event **events;
		int eventlistSize=100;
		void updateEventList();
};
