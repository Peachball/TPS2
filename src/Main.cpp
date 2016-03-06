#include <SDL2/SDL.h>
#include <iostream>
#include "Graphics.h"
#include "GameScreen.h"
#include "StartScreen.h"

int main(){
	graphics::create();

	Screen* scr = new GameScreen(TESTER);

	while( scr != NULL ){
		scr = scr->update();
	}

	graphics::close();

	return 0;
}
