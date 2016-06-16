#include "game/Bullet.h"

SDL_Texture* Bullet::image = NULL;
std::string Bullet::IMAGE_LOC = "bullet.jpeg";

Bullet::Bullet(GameManager* m): GameObject(m){
}

void Bullet::display(){

}

void Bullet::gameUpdate(Uint32 time){

}

Bullet::~Bullet(){

}

void Bullet::init(){
}

void Bullet::del(){
}
