#include "GameScreen.h"

GameScreen::GameScreen(GAMEMODE mode){
	gm = mode;
	this->manager.startGame();
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
	return this;
}

GameScreen::~GameScreen(){
}
