#include "game/Bullet.h"

SDL_Texture* Bullet::image = NULL;
std::string Bullet::IMAGE_LOC = "bullet.png";
std::mutex Bullet::imageLock;

Bullet::Bullet(GameManager* m, float x, float y, float direction): GameObject(m){
	init();
	xpos = x;
	ypos = y;
	int w, h;
	imageLock.lock();
	SDL_QueryTexture(image, NULL, NULL, &w, &h);
	imageLock.unlock();
	src.x = 0;
	src.y = 0;
	src.w = w;
	src.h = h;
	speed = 1;
	this->direction = direction;
}

void Bullet::display(){
	using namespace graphics;
	dest.x = (int) xpos;
	dest.y = (int) ypos;
	dest.w = 100;
	dest.h = 100;
	imageLock.lock();
	if(SDL_RenderCopy(render, image, &src, &dest) < 0){
		logError();
	}
	imageLock.unlock();
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
	imageLock.lock();
	image = graphics::loadTexture(IMAGE_LOC.c_str());
	imageLock.unlock();

	initialized = true;
}

void Bullet::del(){
	graphics::close(image);
}
