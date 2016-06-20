#include "game/Bullet.h"

SDL_Texture* Bullet::image = NULL;
SDL_Surface* Bullet::surface = NULL;
std::string Bullet::IMAGE_LOC = "bullet.png";
std::mutex Bullet::imageLock;

Bullet::Bullet(GameManager* m, float x, float y, float direction): GameObject(m){
	init();
	xpos = x;
	ypos = y;
	src.x = 0;
	src.y = 0;
	src.w = 100;
	src.h = 100;
	speed = 0.1;
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
	this->unserialize(this->serialize());
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
	surface = graphics::loadImage(IMAGE_LOC.c_str());

	initialized = true;
	std::cout<<"Bullet initialized\n";
}

void Bullet::del(){
	graphics::close(image);
	graphics::close(surface);
	std::cout<<"Bullet deleted\n";
}

NetworkManager::Message Bullet::serialize(){
	NetworkManager::Message m;
	m.m = std::shared_ptr<char>(new char[BULLETDATA_SIZE]);
	m.len = BULLETDATA_SIZE;
	char* data = m.m.get();
	memcpy(data, &id, sizeof(id));
	data += sizeof(id);
	memcpy(data, &xpos, sizeof(xpos));
	data += sizeof(xpos);
	memcpy(data, &ypos, sizeof(ypos));
	return m;
}

void Bullet::unserialize(NetworkManager::Message m){
	if(m.len != BULLETDATA_SIZE){
		logError("Something has gone horribly wrong");
		return;
	}
	char* data = m.m.get();
	uint32_t temp_id;
	memcpy(&temp_id, data, sizeof(temp_id));
	data += sizeof(temp_id);
	if(temp_id != id){
		logError("Id mismatch for bullet");
		return;
	}

	memcpy(&xpos, data, sizeof(xpos));
	data += sizeof(xpos);
	memcpy(&ypos, data, sizeof(ypos));
}
