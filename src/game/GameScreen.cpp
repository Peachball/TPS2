#include "GameScreen.h"

GameScreen::GameScreen(GAMEMODE mode){
	gm = mode;
	switch(gm){
		case SINGLEPLAYER:
			break;
		case MULTIPLAYER_CLIENT:
			break;
		case MULTIPLAYER_SERVER:
			break;
		case TESTER:
			manager.startGame();
			break;
	}
}

Screen* GameScreen::update(){
	manager.handleEvents();
	switch(gm){
		case SINGLEPLAYER:
			break;
		case MULTIPLAYER_CLIENT:
			break;
		case MULTIPLAYER_SERVER:
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
