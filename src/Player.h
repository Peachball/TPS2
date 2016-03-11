#pragma once
#include "GameObject.h"
#include "Graphics.h"
#include <SDL2/SDL.h>

const std::string DEFAULT_NAME="player-topdown.png";

class Player : public GameObject{
	public:
		Player();
		virtual void display();
		~Player();
	private:
		SDL_Texture *image;
};
