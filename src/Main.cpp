#include <SDL2/SDL.h>
#include <iostream>

#include "Graphics.h"
#include "game/GameScreen.h"
#include "StartScreen.h"
#include "gui/GUIHandler.h"

int main(){
	std::cout<<"Program just started\n";
	int status = graphics::create();

	std::cout<<status<<'\n';

	Screen* scr = new GameScreen(TESTER);

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
