#include "GameManager.h"


GameManager::GameManager(){
	//Default settings here
	Player * p = new Player();
	objects.push_back(p);
}

GameManager::~GameManager(){
	while(!objects.empty()){
		GameObject* o = objects.back();
		delete o;
		objects.pop_back();
	}
}

void GameManager::startGame(){
	std::thread(renderloop);
	SDL_Delay(1000);
}

void GameManager::renderloop(){
	while(status = 1){
		for(std::vector<GameObject*>::iterator it=objects.begin(); it != objects.end(); ++it){
			(*it)->display();
		}
	}
}
