#include "game/Soldier76.h"

const std::string Soldier76::DEFAULT_NAME="sol76.png";
const int Soldier76::HP_DIST = 5;
const int Soldier76::HP_HEIGHT = 20;

Soldier76::Soldier76(GameManager* m, float x, float y): Player(m, x, y){
	init();
	src.x = 0;
	src.y = 0;
	src.w = IMG_WIDTH;
	src.h = IMG_HEIGHT;
	flip = SDL_FLIP_NONE;
}

void Soldier76::display(){
	using namespace graphics;
	dest.x = (int) xpos - dest.w / 2.0;
	dest.y = (int) ypos - dest.h / 2.0;
	if(SDL_RenderCopyEx(render, image, &src, &dest, 0, NULL, flip)){
		logError();
	}
	displayUI();
}

void Soldier76::displayUI(){
	//Display Health bar
	{
		SDL_Rect hbar = {
			(int) dest.x,
			(int) (dest.y - HP_DIST - HP_HEIGHT),
			dest.w,
			HP_HEIGHT
		};
		
		SDL_SetRenderDrawColor(graphics::render, 0x7C, 0xFC, 0x00, 0xFF);
		SDL_RenderFillRect(graphics::render, &hbar);
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

	if(mouse_x > xpos){
		flip = SDL_FLIP_HORIZONTAL;
	}
	else{
		flip = SDL_FLIP_NONE;
	}

	if(keystate[SHOOT_BUTTON]){
		float direction = 0;
		float correction = 0;
		if(xpos - mouse_x > 0){
			correction = M_PI;
		}
		direction = atan((ypos - mouse_y) / (xpos - mouse_x)) + correction;
		Bullet* b = new Bullet(manager, xpos, ypos, direction);
		manager->addObject(b);
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
