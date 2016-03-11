#include "GameManager.h"


GameManager::GameManager(){
	//Default settings here
	Player * p = new Player();
}

GameManager::~GameManager(){
	while(!objects.empty()){
		GameObject* o = objects.back();
		delete o;
		objects.pop_back();
	}
}
