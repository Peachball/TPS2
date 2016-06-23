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

	keystate = new bool[KEYS]();

	mouse_x = 0;
	mouse_y = 0;
}

void Player::display(){
	using namespace graphics;
	dest.x = (int) (xpos - src.w / 2.0);
	dest.y = (int) (ypos - src.h / 2.0);
	if(SDL_RenderCopy(render, Player::image, &src, &dest)<0){
		logError();
	}
}

void Player::getInput(){
	int numkeys;
	const Uint8* key = SDL_GetKeyboardState(&numkeys);
	keystate[DOWN] = key[SDL_SCANCODE_DOWN];
	keystate[RIGHT] = key[SDL_SCANCODE_RIGHT];
	keystate[LEFT] = key[SDL_SCANCODE_LEFT];
	keystate[UP] = key[SDL_SCANCODE_UP];
	if(SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT)){
		keystate[SHOOT_BUTTON] = true;
	}
	else{
		keystate[SHOOT_BUTTON] = false;
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
	if(keystate[SHOOT_BUTTON]){
		float correction = 0;
		if(xpos - mouse_x > 0){
			correction = M_PI;
		}
		shoot(atan((ypos - mouse_y) / (xpos - mouse_x)) + correction);
	}

	float relx = mouse_x - xpos;
	float rely = mouse_y - ypos;
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
	if(keystate[DOWN]){
		ypos += movementSpeed * time;
	}
	if(keystate[UP]){
		ypos -= movementSpeed * time;
	}
	if(keystate[LEFT]){
		xpos -= movementSpeed * time;
	}
	if(keystate[RIGHT]){
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

NetworkManager::Message Player::serialize(char* buffer){
	NetworkManager::Message m;
	m.m = buffer;
	m.len = PLAYERDATA_SIZE;
	char* data = m.m;
	memcpy(data, &id, sizeof(id));
	data += sizeof(id);
	char type = PLAYER;
	memcpy(data, &type, sizeof(type));
	data += sizeof(type);
	memcpy(data, &xpos, sizeof(xpos));
	data += sizeof(xpos);
	memcpy(data, &ypos, sizeof(ypos));
	return m;
}

void Player::unserialize(NetworkManager::Message m){
	if(PLAYERDATA_SIZE != m.len){
		logError("Corrupted udp packet");
		std::cout<<"Packet size was: "<<m.len<<'\n';
		return;
	}
	char* data = m.m;

	uint32_t test_id;
	memcpy(&test_id, data, sizeof(test_id));
	data += sizeof(test_id);
	if(test_id != id){
		logError("This is not the player you are looking for");
	}

	char type;
	memcpy(&type, data, sizeof(type));
	data += sizeof(type);

	if(type != PLAYER){
		logError("Not a player, bro");
	}

	//Outline:
	//		Store X and Y position (and thats it)
	memcpy(&xpos, data, sizeof(xpos));
	data += sizeof(xpos);
	memcpy(&ypos, data, sizeof(ypos));
}

void Player::getInput(NetworkManager::Message m){
	if(m.len != PLAYER_INPUT_SIZE){
		logError("Data size is incorrect for player...");
		std::cout<<"Data size was: "<<m.len<<'\n';
		return;
	}

	char* data = m.m;

	memcpy(keystate, data, KEYS);
	data += KEYS;
	memcpy(&mouse_x, data, sizeof(uint32_t));
	data += sizeof(uint32_t);
	memcpy(&mouse_y, data, sizeof(uint32_t));
}

NetworkManager::Message Player::serializeInput(char* loc){
	NetworkManager::Message m;
	m.m = loc;
	char* data = loc;
	memcpy((data), keystate, KEYS);
	data += KEYS;
	memcpy(data, &mouse_x, sizeof(mouse_x));
	data += sizeof(mouse_x);
	memcpy(data, &mouse_y, sizeof(mouse_y));
	
	m.len = PLAYER_INPUT_SIZE;
	return m;
}
