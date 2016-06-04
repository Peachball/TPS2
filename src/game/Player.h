#pragma once
#include "GameObject.h"
#include "Graphics.h"
#include <SDL2/SDL.h>

const std::string DEFAULT_NAME="player-topdown.png";

class Player : public GameObject{
	public:
		Player(float x=0, float y=0);
		virtual void display();
		~Player();
	private:
		SDL_Texture *image;
};