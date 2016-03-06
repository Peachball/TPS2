#pragma once
#include <iostream>
#include <SDL2/SDL.h>

class Screen{
	public:
		virtual Screen* update()=0;
};
