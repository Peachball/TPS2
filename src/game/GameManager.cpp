#include "GameManager.h"
#include "game/GameObject.h"
#include "game/Player.h"
#include "game/Bullet.h"

GameManager::GameManager(){
	//Default settings here
	Player* p = new Player(this);
	localPlayer = p;
	objects.push_back(p);
}

GameManager::~GameManager(){
	while(!objects.empty()){
		GameObject* o = objects.back();
		delete o;
		objects.pop_back();
	}

	if(eventThread != NULL){
		delete eventThread;
	}
	if(gameThread != NULL){
		delete gameThread;
	}

	gameThread = eventThread = NULL;
}

void GameManager::startGame(){

	//Initalize debug mode stuff

	//Debugging purposes only currently
	status = GameManager::State::DURING;

	eventThread  = new std::thread(&GameManager::eventHandler, this);
	gameThread   = new std::thread(&GameManager::manageGame, this);

}

void GameManager::endGame(){
	status = GameManager::State::END;

	eventThread->join();
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
		for(int i = 0; i < objects.size(); i++){
			GameObject* g = objects[i];
			g->gameUpdate(prevTime+1);
		}

		prevTime = SDL_GetTicks() - lastTime;
		lastTime = SDL_GetTicks();

		render();
	}

}

void GameManager::eventHandler(){
	SDL_Event event;
	std::cout<<"Polling events\n";
	while(status != GameManager::State::END){
		while(SDL_PollEvent(&event)){
			localPlayer->getInput(&event);

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
	for(std::vector<GameObject*>::iterator it=objects.begin();
			it != objects.end(); ++it){
		(*it)->display();
	}
	graphics::update();

}
/*
 * Display Current state
*/
void GameManager::renderloop(){
}
