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
			//These are overarching game commands
			UPDATE, ADD_PLAYER, SET_PLAYER, GET_PLAYER, SET_PLAYER_INPUT
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
		struct EndpointComparator{
			bool operator()(const asio::ip::udp::endpoint& e1,
					const asio::ip::udp::endpoint& e2) const;
		};
		std::map<asio::ip::udp::endpoint, int /*,EndpointComparator*/> client_players;

		//Client commands
		void update_gamestate(NetworkManager::Message m);
		void request_add_player(NetworkManager* net);
		void game_handler(NetworkManager* net, const asio::error_code& error, std::size_t bytes);
		void join_server(NetworkManager* net);
		Player* localPlayer;
		void send_server_player_input(NetworkManager* net);
		void set_player_input(int id, NetworkManager::Message m);


	private:
		void render();
		void manageGame();
		std::thread* gameThread;
		std::list<GameObject*> objects;

		const int HEADER_SIZE=2;

		char* __buffer;
		asio::ip::udp::endpoint remote;
		inline GameObject* get_object(int id);
};

#endif
