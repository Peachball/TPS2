#pragma once
#include <thread>
#include "Graphics.h"

class GUIHandler{
	public:
		enum ListenState{
			DELETE,
			SAME
		};

		void start();
		void stop();
		~GUIHandler();
		void addEventListener();

	private:
		SDL_Event **events;
		int eventlistSize=100;
		void updateEventList();
};
