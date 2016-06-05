#include "GameScreen.h"

GameScreen::GameScreen(GAMEMODE mode){
	gm = mode;
	manager.startGame();
}

Screen* GameScreen::update(){
	switch(gm){
		case SINGLEPLAYER:
			break;
		case MULTIPLAYER:
			break;
		case TESTER:
			break;
	}
	if(manager.status == GameManager::State::END){
		return NULL;
	}
	return this;
}

GameScreen::~GameScreen(){
	std::cout<<"gamescreen destructor called\n";
}
