#include "Player.h"

const std::string Player::DEFAULT_TEXTURE_LOC="player-topdown.png";
SDL_Texture* Player::image = NULL;

Player::Player(GameManager* m, float x, float y):GameObject(m){
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
	dest.x = (int) xpos;
	dest.y = (int) ypos;
	if(SDL_RenderCopy(render, image, &src, &dest)<0){
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
	graphics::close(image);

	delete [] keystate;
	keystate = NULL;

}

void Player::gameUpdate(Uint32 time){
	if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
		
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
	image = graphics::loadTexture(Player::DEFAULT_TEXTURE_LOC.c_str());

	initialized = true;
}
