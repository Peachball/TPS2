#include <SDL2/SDL.h>
#undef main
#include <iostream>
#include "Graphics.h"
#include "game/GameScreen.h"
#include "StartScreen.h"
#include "gui/GUIHandler.h"

int main(int argc, char* argv[]){
	graphics::create();

	GAMEMODE default_mode = TESTER;

	if(argc >= 2){
		//Debug Mode
		if(std::string(argv[1]) == "ser"){
			default_mode = MULTIPLAYER_SERVER;
		}
		else if(std::string(argv[1]) == "cli"){
			default_mode = MULTIPLAYER_CLIENT;
		}
		else{
			logError("Unknown command");
		}
	}

	Screen* scr = new GameScreen(default_mode);

	GUIHandler gui;

	Screen* before = scr;
	while( scr != NULL ){
		scr = scr->update();
		if(before != scr){
			delete before;
		}
		before = scr;
	}

	graphics::closeSDL();

	return 0;
}
