#pragma once
#include <map>
#include <iostream>
#include "Graphics.h"
#include "Screen.h"
#include "GUIHandler.h"

using namespace graphics;

class StartScreen: public Screen{
	public:
		StartScreen();
		virtual Screen* update();
		virtual ~StartScreen();
	private:
		std::map<std::string, SDL_Surface*> images;
		void loadImages();
		GUIHandler gui;
};
