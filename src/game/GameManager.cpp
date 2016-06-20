#include "GameManager.h"
#include "game/GameObject.h"
#include "game/Player.h"
#include "game/Bullet.h"

GameManager::GameManager(){
	curId = 0;
}

void GameManager::setLocalPlayer(Player* p){
	localPlayer = p;
	//Test if p is not in objects
	if(std::find(objects.begin(), objects.end(), p) == objects.end()){
		p->id = curId++;
		objects.push_back(p);
	}
}

GameManager::~GameManager(){
	while(!objects.empty()){
		GameObject* o = objects.back();
		delete o;
		objects.pop_back();
	}

	if(gameThread != NULL){
		delete gameThread;
	}

	gameThread = NULL;
}

void GameManager::startGame(){

	//Initalize debug mode stuff

	//Debugging purposes only currently
	status = GameManager::State::DURING;

	gameThread   = new std::thread(&GameManager::manageGame, this);

}

void GameManager::endGame(){
	status = GameManager::State::END;

	gameThread->join();

	Player::del();
	Bullet::del();

	std::cout<<"Game ended successfully\n";
}

//Do all the game logic stuff here such as:
//	Get rid of dead people
//	Collision detection
//	Manage game status
void GameManager::manageGame(){
	Uint32 prevTime = 0;
	while(status != GameManager::State::END){
		Uint32 lastTime= SDL_GetTicks();
		for(GameObject* g : objects){
			g->gameUpdate(prevTime+1);
		}
		render();

		prevTime = SDL_GetTicks() - lastTime;
		lastTime = SDL_GetTicks();
	}

}

void GameManager::handleEvents(){
	SDL_Event event;
	std::cout<<"Polling events\n";
	while(status != GameManager::State::END){
		while(SDL_PollEvent(&event)){
			if(localPlayer != NULL){
				localPlayer->getInput(&event);
			}

			switch(event.type){
				case SDL_WINDOWEVENT:
					switch(event.window.event){
						case SDL_WINDOWEVENT_CLOSE:
							status = GameManager::State::END;
							break;
					}
					break;
			}
		}
	}

	std::cout<<"Stopped polling events\n";
}

void GameManager::render(){
	graphics::clear();
	for(GameObject* it : objects){
		it->display();
	}
	graphics::update();
}

void GameManager::addObject(GameObject* g){
	g->id = curId++;
	objects.push_back(g);
}

void GameManager::removeObject(GameObject* g){
	objects.remove(g);
	delete g;
	g = NULL;
}

void GameManager::broadcast_gamestate(NetworkManager* net){
	if(net->mode != NetworkManager::SERVER){
		logError("manager is not in correct mode lol");
		return;
	}
	for(GameObject* g : objects){
		net->broadCastMessage(g->serialize());
	}
}

void GameManager::update_gamestate(NetworkManager::Message m){
	if(m.len <= 4){
		logError("Message is corrrupted");
		return;
	}
	//Convert 4 bytes to uint32
	//Big endian
	uint32_t temp_id = 0;
	temp_id = (m.m[0] << 24) | (m.m[1] << 16) | (m.m[2] << 8) |  (m.m[3]);

	//Linear search to find object with id
	for(GameObject* g : objects){
		if(g->id == temp_id){
			g->unserialize(m);
			break;
		}
	}
}
