#include "GameScreen.h"

GameScreen::GameScreen(GAMEMODE mode){
	gm = mode;
	manager.startGame();
}

Screen* GameScreen::update(){
	manager.handleEvents();
	switch(gm){
		case SINGLEPLAYER:
			break;
		case MULTIPLAYER:
			break;
		case TESTER:
			break;
	}
	if(manager.status == GameManager::State::END){
		manager.endGame();
		return NULL;
	}
	return this;
}

GameScreen::~GameScreen(){
}
