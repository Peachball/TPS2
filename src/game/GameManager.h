#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <algorithm>
#include <iostream>
#include <list>
#include <thread>
#include <memory>
#include <map>

#include "NetworkManager.h"
#include "Graphics.h"

//Include files are in cpp file
class GameObject;
class Player;

class GameManager{
	public:
		enum Command{
			UPDATE, ADD_PLAYER, SET_PLAYER, GET_PLAYER
		};
		enum State{
			BEFORE,
			DURING,
			PAUSED,
			END
		};

		GameManager();
		~GameManager();

		//Start game, as in start rendering/do game logic
		void startGame();
		void endGame();

		void addObject(GameObject* g);
		void removeObject(GameObject* g);

		void handleEvents();

		//Server commands
		State status;
		void setLocalPlayer(Player* p);
		void setLocalPlayer(int id);
		void broadcast_gamestate(NetworkManager* net);
		int curId;
		std::map<asio::ip::udp::endpoint, int> client_players;

		//Client commands
		void update_gamestate(NetworkManager::Message m);
		void request_add_player(NetworkManager* net);
		void game_handler(NetworkManager* net, const asio::error_code& error, std::size_t bytes);
		void join_server(NetworkManager* net);


	private:
		void render();
		void manageGame();
		std::thread* gameThread;
		Player* localPlayer;
		std::list<GameObject*> objects;

		const int HEADER_SIZE=2;
};

#endif
