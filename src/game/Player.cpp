#include "Player.h"

Player::Player(float x, float y){
	image = graphics::loadTexture(DEFAULT_NAME.c_str());
	xpos = x;
	ypos = y;

	//Select portion of the texture
	src.x = 0;
	src.y = 0;
	src.w = 143;
	src.h = 116;
	dest = src;
	dest.x = 100;
}

void Player::display(){
	using namespace graphics;
	if(SDL_RenderCopy(render, image, &src, &dest)<0){
		logError();
	}
}

void Player::getInput(const SDL_Event* event){
	switch(event->type){
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym){
				case SDLK_DOWN:
					std::cout<<"Down button pressed\n";
					break;
				case SDLK_UP:
					break;
				case SDLK_RIGHT:
					break;
				case SDLK_LEFT:
					break;
			}
			break;
	}
}

Player::~Player(){
	graphics::close(image);
}
