#include "game/Bullet.h"

std::string Bullet::BULLET_TEXTURE_LOC = "bullet.jpeg";
SDL_Texture* Bullet::texture = graphics::loadTexture(Bullet::BULLET_TEXTURE_LOC.c_str());

Bullet::Bullet(GameManager* m):GameObject(m){

}

Bullet::~Bullet(){

}

void Bullet::display(){

}

void Bullet::gameUpdate(Uint32 time){

}

void Bullet::init(){
}
