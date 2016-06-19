#include "GameScreen.h"

GameScreen::GameScreen(GAMEMODE mode){
	net = NULL;
	gm = mode;
	switch(gm){
		case SINGLEPLAYER:
			break;
		case MULTIPLAYER_CLIENT:
			try{
				net = new NetworkManager(NetworkManager::MODE::CLIENT);
				net->connect_to_server("localhost:2000");
			}
			catch(std::exception &e){
				std::cout<<e.what()<<'\n';
			}
			break;
		case MULTIPLAYER_SERVER:
			try{
			net = new NetworkManager(NetworkManager::MODE::SERVER);
			net->create_local_server(2000);
			}
			catch(std::exception &e){
				std::cout<<e.what()<<'\n';
			}
			break;
		case TESTER:
			break;
	}

	manager.startGame();
}

Screen* GameScreen::update(){
	manager.handleEvents();
	if(manager.status == GameManager::State::END){
		manager.endGame();
		return NULL;
	}
	return this;
}

GameScreen::~GameScreen(){
	if(net != NULL){
		delete net;
	}
}
