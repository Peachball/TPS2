#pragma once
#include "GameObject.h"
#include "Graphics.h"
#include <SDL2/SDL.h>

static const std::string DEFAULT_NAME="player-topdown.png";

class Player : public GameObject{
	public:
		Player(float x=0, float y=0);
		virtual void display();
		virtual ~Player();
		void getInput(const SDL_Event* event);

		virtual void gameUpdate();

	private:
		SDL_Texture *image;
		SDL_Rect src;
		SDL_Rect dest;

		bool* keystate;

		float xpos;
		float ypos;
};
