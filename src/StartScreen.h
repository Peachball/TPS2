#pragma once
#include <iostream>
#include "Screen.h"

std::map<std::string, SDL_Surface*> images;

class StartScreen: public Screen{
	public:
		StartScreen();
		virtual Screen* update();
		virtual ~Screen();
	private:
		void loadImages();
};
