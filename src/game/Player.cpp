#include "Player.h"

SDL_Texture* Player::image = NULL;
const std::string Player::DEFAULT_NAME ="player-topdown.png";

Player::Player(GameManager* m, float x, float y) : GameObject(m){
	init();
	xpos = x;
	ypos = y;

	//Select portion of the texture
	src.x = 0;
	src.y = 0;
	src.w = 143;
	src.h = 116;
	dest = src;
	dest.x = 100;

	keystate = new bool[300];
}

void Player::display(){
	using namespace graphics;
	dest.x = (int) (xpos - src.w / 2.0);
	dest.y = (int) (ypos - src.h / 2.0);
	if(SDL_RenderCopy(render, Player::image, &src, &dest)<0){
		logError();
	}
}

void Player::getInput(const SDL_Event* event){

	switch(event->type){
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym){
				case SDLK_DOWN:
					keystate[SDL_SCANCODE_DOWN] = true;
					break;
				case SDLK_UP:
					keystate[SDL_SCANCODE_UP] = true;
					break;
				case SDLK_RIGHT:
					keystate[SDL_SCANCODE_RIGHT] = true;
					break;
				case SDLK_LEFT:
					keystate[SDL_SCANCODE_LEFT] = true;
					break;
			}
			break;

		case SDL_KEYUP:
			switch(event->key.keysym.sym){
				case SDLK_DOWN:
					keystate[SDL_SCANCODE_DOWN] = false;
					break;
				case SDLK_UP:
					keystate[SDL_SCANCODE_UP] = false;
					break;
				case SDLK_RIGHT:
					keystate[SDL_SCANCODE_RIGHT] = false;
					break;
				case SDLK_LEFT:
					keystate[SDL_SCANCODE_LEFT] = false;
					break;
			}

			break;
	}
}

Player::~Player(){
	delete [] keystate;
	keystate = NULL;
}

void Player::shoot(float direction){
	Bullet* b = new Bullet(manager, xpos, ypos, direction);
	manager->addObject(b);
}

void Player::gameUpdate(Uint32 time){
	this->unserialize(this->serialize());
	//Detect mouse button status
	int x;
	int y;
	if(SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)){
		float correction = 0;
		if(xpos - x > 0){
			correction = M_PI;
		}
		shoot(atan((ypos - y) / (xpos - x)) + correction);
	}

	float relx = x - xpos;
	float rely = y - ypos;
	//Right side
	if(relx > abs(rely)){
		src.y = src.h * 3;
	}
	
	//Top side
	if(rely < -abs(relx)){
		src.y = src.h * 1;
	}

	//Left Side
	if(relx < -abs(rely)){
		src.y = src.h * 2;
	}

	//Bottom
	if(rely > abs(relx)){
		src.y = src.h * 0;
	}
	if(keystate[SDL_SCANCODE_DOWN]){
		ypos += movementSpeed * time;
	}
	if(keystate[SDL_SCANCODE_UP]){
		ypos -= movementSpeed * time;
	}
	if(keystate[SDL_SCANCODE_LEFT]){
		xpos -= movementSpeed * time;
	}
	if(keystate[SDL_SCANCODE_RIGHT]){
		xpos += movementSpeed * time;
	}
}

void Player::init(){
	static bool initialized = false;
	if(initialized){
		return;
	}
	image = graphics::loadTexture(DEFAULT_NAME.c_str());
	initialized = true;
}

void Player::del(){
	graphics::close(image);
}

NetworkManager::Message Player::serialize(){
	NetworkManager::Message m;
	m.m = std::shared_ptr<char>(new char[PLAYERDATA_SIZE]);
	m.len = PLAYERDATA_SIZE;
	char* data = m.m.get();
	memcpy(data, &id, sizeof(id));
	data += sizeof(id);
	memcpy(data, &xpos, sizeof(xpos));
	data += sizeof(xpos);
	memcpy(data, &ypos, sizeof(ypos));
	return m;
}

void Player::unserialize(NetworkManager::Message m){
	if(PLAYERDATA_SIZE != m.len){
		logError("Corrupted udp packet");
		return;
	}
	char* data = m.m.get();

	uint32_t test_id;
	memcpy(&test_id, data, sizeof(uint32_t));
	if(test_id != id){
		logError("This is not the player you are looking for");
	}

	//Outline:
	//		Store X and Y position (and thats it)
	data += sizeof(test_id);
	memcpy(&xpos, data, sizeof(xpos));
	data += sizeof(xpos);
	memcpy(&ypos, data, sizeof(ypos));
}
