#include <SDL2/SDL.h>
#include <iostream>
#include "Graphics.h"
#include "StartScreen.h"

int main(){
	graphics::create();

	Screen* scr = new StartScreen();

	while( scr != NULL ){
		scr = scr->update();
	}

	graphics::close();

	return 0;
}
