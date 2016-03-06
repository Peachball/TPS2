#include "GameScreen.h"

GameScreen::GameScreen(GAMEMODE mode){
	gm = mode;
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
	return NULL;
}

GameScreen::~GameScreen(){
}
