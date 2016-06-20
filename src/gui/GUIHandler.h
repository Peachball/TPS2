#ifndef _GUIHANDLER_H
#define _GUIHANDLER_H
#include <thread>
#include "Graphics.h"

class GUIHandler{
	public:
		enum ListenState{
			DEL,
			SAME
		};

		GUIHandler();

		void start();
		void stop();
		virtual ~GUIHandler();
		void addEventListener();

	private:
		SDL_Event **events;
		int eventlistSize=100;
		void updateEventList();
};

#endif
