#include "GameManager.h"


GameManager::GameManager(){
	//Default settings here
//	Player* p = new Player();
//	objects.push_back(p);
}

GameManager::~GameManager(){
	while(!objects.empty()){
		GameObject* o = objects.back();
		delete o;
		objects.pop_back();
	}
}

void GameManager::startGame(){

	//Initalize debug mode stuff

	//Debugging purposes only currently
	status = GameManager::State::DURING;
	std::thread r(&GameManager::renderloop, this);
	std::thread event(&GameManager::eventHandler, this);
	SDL_Delay(1);

	status = GameManager::State::END;
	r.join();
	event.join();

	std::cout<<"Exited game\n";
}

void GameManager::eventHandler(){
	SDL_Event event;
	std::cout<<"Polling events\n";
	while(status != GameManager::State::END){
		while(SDL_PollEvent(&event)){
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
}
/*
 * Display Current state
*/
void GameManager::renderloop(){
	std::cout<<"Size of render loop "<<objects.size()<<'\n';
	while(status == GameManager::State::DURING){
		for(std::vector<GameObject*>::iterator it=objects.begin();
				it != objects.end(); ++it){
			(*it)->display();
		}
	}
	std::cout<<"Exited rendering loop\n";
}
