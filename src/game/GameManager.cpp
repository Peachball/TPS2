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

	//Debugging purposes only currently
	std::thread r(&GameManager::renderloop, this);
	SDL_Delay(1000);
	status = GameManager::State::DURING;

	r.join();
}

/*
 * Display Current state
*/
void GameManager::renderloop(){
	std::cout<<"Size of render loop "<<objects.size()<<'\n';
	while(status == GameManager::State::DURING){
		for(std::vector<GameObject*>::iterator it=objects.begin(); it != objects.end(); ++it){
			(*it)->display();
		}
	}
}
