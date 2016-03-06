#include "Button.h"

Button::Button(std::string filepath, int xpos, int ypos, double xscale, double yscale){
	image = loadTexture(filepath.c_str());

	int w, h;
	SDL_QueryTexture(image, NULL, NULL, &w, &h);

	dstRect.x = xpos;
	dstRect.y = ypos;
	dstRect.w = w * xscale;
	dstRect.h = h * yscale;
}

void Button::setListener(void (*function)(SDL_Event*)){
	callbacks.push_back(function);
}

bool Button::display(){
	int status = SDL_RenderCopy(render, image, NULL, &dstRect);
	if( status < 0){
		std::cout<<"Failed to draw button image\n";
		std::cout<<"Error: "<<SDL_GetError()<<'\n';
	}
	return status < 0;
}

void Button::processEvent(const SDL_Event* event){
}

Button::~Button(){
	closeTexture(image);
}
