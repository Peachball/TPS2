#include "GameManager.h"


GameManager::GameManager(){
	//Default settings here
	Player* p = new Player();
	localPlayer = p;
	objects.push_back(p);
}

GameManager::~GameManager(){
	while(!objects.empty()){
		GameObject* o = objects.back();
		delete o;
		objects.pop_back();
	}

	if(renderThread != NULL){
		delete renderThread;
	}
	if(eventThread != NULL){
		delete eventThread;
	}
	if(gameThread != NULL){
		delete gameThread;
	}

	renderThread = gameThread = eventThread = NULL;
}

void GameManager::startGame(){

	//Initalize debug mode stuff

	//Debugging purposes only currently
	status = GameManager::State::DURING;

	renderThread = new std::thread(&GameManager::renderloop, this);
	eventThread  = new std::thread(&GameManager::eventHandler, this);
	gameThread   = new std::thread(&GameManager::manageGame, this);

}

void GameManager::endGame(){
	status = GameManager::State::END;

	renderThread->join();
	eventThread->join();
	gameThread->join();

	std::cout<<"Game ended successfully\n";
}

//Do all the game logic stuff here such as:
//	Get rid of dead people
//	Collision detection
//	Manage game status
void GameManager::manageGame(){
	Uint32 lastTime= SDL_GetTicks();
	while(status != GameManager::State::END){
		for(int i = 0; i < objects.size(); i++){
			GameObject* g = objects[i];
			g->gameUpdate(SDL_GetTicks() - lastTime);
		}
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
/*
 * Display Current state
*/
void GameManager::renderloop(){
	std::cout<<"Size of render loop "<<objects.size()<<'\n';
	graphics::clear();
	while(status == GameManager::State::DURING){
		graphics::clear();
		for(std::vector<GameObject*>::iterator it=objects.begin();
				it != objects.end(); ++it){
			(*it)->display();
		}
		graphics::update();
	}
	std::cout<<"Exited rendering loop\n";
}
