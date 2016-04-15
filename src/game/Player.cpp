#include "Player.h"

Player::Player(float x, float y){
	image = graphics::loadTexture(DEFAULT_NAME.c_str());
	xpos = x;
	ypos = y;
}

void Player::display(){
	using namespace graphics;
	if(SDL_RenderCopy(render, image, NULL, NULL)<0){
		logError();
	}
}

Player::~Player(){
	graphics::closeTexture(image);
}
