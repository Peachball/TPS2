#include <SDL2/SDL.h>
#include <iostream>
#include "Graphics.h"
#include "game/GameScreen.h"
#include "StartScreen.h"
#include "GUIHandler.h"

int main(){
	graphics::create();

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

	graphics::close();

	return 0;
}
