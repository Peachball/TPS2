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
				net->reset();
				manager.game_handler(net, asio::error_code(), 0);
				net->startClientMessageThreads();
				manager.join_server(net);
				manager.request_add_player(net);
			}
			catch(std::exception &e){
				std::cout<<e.what()<<'\n';
			}
			break;
		case MULTIPLAYER_SERVER:
			{
				/*
				Player* p = new Player(&manager);
				manager.setLocalPlayer(p);
				*/
				try{
					net = new NetworkManager(NetworkManager::MODE::SERVER);
					net->create_local_server(2000);
					manager.game_handler(net, asio::error_code(), 0);
					net->listen();
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

	if(gm == MULTIPLAYER_SERVER){
		manager.broadcast_gamestate(net);
	}
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
