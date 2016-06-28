#include "game/Soldier76.h"

const std::string Soldier76::DEFAULT_NAME="sol76.png";

Soldier76::Soldier76(GameManager* m, float x, float y): Player(m, x, y){
	init();
	src.x = 0;
	src.y = 0;
	src.w = IMG_WIDTH;
	src.h = IMG_HEIGHT;
}

void Soldier76::display(){
	using namespace graphics;
	dest.x = (int) xpos;
	dest.y = (int) ypos;
	if(!SDL_RenderCopy(render, image, &src, &dest)){
		logError();
	}
}

void Soldier76::gameUpdate(Uint32 time){
	if(keystate[DOWN]){
		ypos += movementSpeed*time;
	}
	if(keystate[RIGHT]){
		xpos += movementSpeed*time;
	}
	if(keystate[LEFT]){
		xpos -= movementSpeed*time;
	}
	if(keystate[UP]){
		ypos -= movementSpeed*time;
	}

}

void Soldier76::init(){
	static bool inited = false;
	if(inited){
		return;
	}
	image = graphics::loadTexture(DEFAULT_NAME.c_str());

	inited = true;
}

void Soldier76::del(){
	static bool deled = false;
	if(deled){
		return;
	}
	graphics::close(image);
	deled = true;
}

NetworkManager::Message Soldier76::serialize(char* buffer){
	NetworkManager::Message m;
	m.m = buffer;
	m.len = SOLDATA_SIZE;
	return m;
}

void Soldier76::unserialize(NetworkManager::Message m){

}

Soldier76::~Soldier76(){
}
