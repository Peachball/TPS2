#include "game/Bullet.h"

std::string Bullet::BULLET_TEXTURE_LOC = "bullet.jpeg";
SDL_Texture* Bullet::texture = NULL;

Bullet::Bullet(GameManager* m):GameObject(m){
	init();
}

Bullet::~Bullet(){

}

void Bullet::display(){

}

void Bullet::gameUpdate(Uint32 time){

}

void Bullet::init(){
	static bool initialized = false;
	if(initialized){
		return;
	}
	texture = graphics::loadTexture(Bullet::BULLET_TEXTURE_LOC.c_str());

	initialized = true;
}
