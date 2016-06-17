#include "game/Bullet.h"

SDL_Texture* Bullet::image = NULL;
std::string Bullet::IMAGE_LOC = "bullet.jpeg";

Bullet::Bullet(GameManager* m, float x, float y, float direction): GameObject(m){
	init();
	xpos = x;
	ypos = y;
	speed = 1;
	this->direction = direction;
}

void Bullet::display(){
	using namespace graphics;
	dest.x = (int) xpos;
	dest.y = (int) ypos;
	if(SDL_RenderCopy(render, image, NULL, &dest) < 0){
		logError();
	}
}

void Bullet::gameUpdate(Uint32 time){
	xpos += speed * time * cos(direction);
	ypos += speed * time * sin(direction);
}

Bullet::~Bullet(){

}

void Bullet::init(){
	static bool initialized = false;
	if(initialized){
		return;
	}
	image = graphics::loadTexture(IMAGE_LOC.c_str());
}

void Bullet::del(){
	graphics::close(image);
}
