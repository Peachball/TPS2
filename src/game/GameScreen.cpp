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
				net->connect_to_server("localhost");
				for(int i = 0; i < 100; i++){
					net->send_server_message("hi!");
				}
			}
			catch(std::exception &e){
				std::cout<<e.what()<<'\n';
			}
			break;
		case MULTIPLAYER_SERVER:
			{
				Player* p = new Player(&manager);
				manager.setLocalPlayer(p);
				try{
					net = new NetworkManager(NetworkManager::MODE::SERVER);
					net->create_local_server(2000);
					net->startClientMessageThreads();
				}
				catch(std::exception &e){
					std::cout<<e.what()<<'\n';
				}
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
