#include "GameManager.h"
#include "game/GameObject.h"
#include "game/Player.h"
#include "game/Bullet.h"

GameManager::GameManager(){
	curId = 1;
}

void GameManager::setLocalPlayer(int id){
	if(localPlayer != NULL){
	if(localPlayer->id == id){
		return;
	}
	}
	for(GameObject* g : objects){
		if(g->id == id){
			localPlayer = static_cast<Player*>(g);
			if(localPlayer == NULL){
				logError("That was not a player...welp we gon crash now");
			}
			return;
		}
	}
}

void GameManager::setLocalPlayer(Player* p){
	if(localPlayer != NULL){
		localPlayer->localPlayer = false;
	}
	localPlayer = p;
	p->localPlayer=true;
	//Test if p is not in objects
	if(std::find(objects.begin(), objects.end(), p) == objects.end()){
		p->id = curId++;
		objects.push_back(p);
	}
}

GameManager::~GameManager(){
	while(!objects.empty()){
		GameObject* o = objects.back();
		delete o;
		objects.pop_back();
	}

	if(gameThread != NULL){
		delete gameThread;
	}

	gameThread = NULL;
}

void GameManager::startGame(){

	//Initalize debug mode stuff

	//Debugging purposes only currently
	status = GameManager::State::DURING;

	gameThread   = new std::thread(&GameManager::manageGame, this);

}

void GameManager::endGame(){
	status = GameManager::State::END;

	gameThread->join();

	Player::del();
	Bullet::del();

	std::cout<<"Game ended successfully\n";
}

//Do all the game logic stuff here such as:
//	Get rid of dead people
//	Collision detection
//	Manage game status
void GameManager::manageGame(){
	Uint32 prevTime = 0;
	while(status != GameManager::State::END){
		Uint32 lastTime= SDL_GetTicks();
		for(GameObject* g : objects){
			g->gameUpdate(prevTime+1);
		}
		render();

		prevTime = SDL_GetTicks() - lastTime;
		lastTime = SDL_GetTicks();
	}

}

void GameManager::handleEvents(){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		if(localPlayer != NULL){
			localPlayer->getInput(&event);
		}

		switch(event.type){
			case SDL_WINDOWEVENT:
				switch(event.window.event){
					case SDL_WINDOWEVENT_CLOSE:
						status = GameManager::State::END;
						break;
				}
				break;
		}
	}

}

void GameManager::render(){
	graphics::clear();
	for(GameObject* it : objects){
		it->display();
	}
	graphics::update();
}

void GameManager::addObject(GameObject* g){
	if(g->id == 0){
		g->id = curId++;
	}
	objects.push_back(g);
}

void GameManager::removeObject(GameObject* g){
	objects.remove(g);
	delete g;
	g = NULL;
}

void GameManager::broadcast_gamestate(NetworkManager* net){
	char* buffer = new char[NetworkManager::PACKET_SIZE];
	if(net->mode != NetworkManager::SERVER){
		logError("manager is not in correct mode lol");
		return;
	}
	for(GameObject* g : objects){
		net->broadcastMessage(g->serialize(buffer));
	}

	delete [] buffer;
	buffer = NULL;
}

void GameManager::update_gamestate(NetworkManager::Message m){
	if(m.len <= 4){
		logError("Message is corrrupted");
		return;
	}
	uint32_t temp_id = 0;
	memcpy(&temp_id, m.m, sizeof(temp_id));

	//Linear search to find object with id
	for(GameObject* g : objects){
		if(g->id == temp_id){
			g->unserialize(m);
			return;
		}
	}

	//Generate the object because if wasn't found
	char type;
	memcpy(&type, m.m+sizeof(temp_id), sizeof(type));
	switch(type){
		case PLAYER:
			{
				Player *p = new Player(this);
				p->id = temp_id;
				p->unserialize(m);
				addObject(p);
			}
			break;
		case BULLET:
			{
				Bullet* b = new Bullet(this);
				b->id = temp_id;
				b->unserialize(m);
				addObject(b);
			}
			break;
	}
}

void GameManager::game_handler(NetworkManager* net, const asio::error_code& error, std::size_t bytes){
	if(net->mode == NetworkManager::SERVER){
		logError("This handler should not be used for servers...");
		return;
	}
	static char* _buffer;
	if(_buffer == NULL){
		_buffer = new char[NetworkManager::PACKET_SIZE];
	}
	static asio::ip::udp::endpoint _endpoint;
	using namespace std::placeholders;
	net->socket->async_receive_from(
			asio::buffer(_buffer, NetworkManager::PACKET_SIZE),
			_endpoint, 0,
			std::bind(&GameManager::game_handler, this, net, _1, _2));

	if(_buffer[0] != NetworkManager::GAME_COMMUNICATE){
		return;
	}
	if(bytes == 0){
		return;
	}

	NetworkManager::Message m;
	m.m = _buffer + 2; //First byte is network stuff, 2 byte is game header
	m.len = bytes - 2; //Cuz I added 2 above

	if(net->mode == NetworkManager::CLIENT){
		switch(_buffer[1]){
			case UPDATE:
				update_gamestate(m);
				break;
			case SET_PLAYER:
				int id;
				memcpy(&id, _buffer+1, sizeof(id));
				setLocalPlayer(id);
				break;
		}
	}
	if(net->mode == NetworkManager::SERVER){
		switch(_buffer[1]){
			case ADD_PLAYER:
				{
					Player* p = new Player(this);
					p->unserialize(m);
					p->id = curId++;
					addObject(p);
					NetworkManager::Message response;
					response.m = new char[NetworkManager::PACKET_SIZE];
					
					//2 because there are 2 headers ("GAMECOMM and SET_PLAYER
					//4 because the id is 4 bytes long
					response.len = 2 + 4;
					client_players.insert(
							std::pair<asio::ip::udp::endpoint, int>
							(_endpoint, p->id));
					response.m[0] = (char) NetworkManager::GAME_COMMUNICATE;
					response.m[1] = (char) SET_PLAYER;
					memcpy(response.m + 2, &(p->id), sizeof(p->id));
					net->send_client_message(response, _endpoint);
					delete [] response.m;
					response.m = NULL;
				}
				break;
			case GET_PLAYER:
				int playerid = 0;
				if(client_players.count(_endpoint) > 0){
					playerid = client_players[_endpoint];
				}
				else{
					break;
				}
				NetworkManager::Message mes;
				mes.m = new char[NetworkManager::PACKET_SIZE];
				mes.m[0] = (char) NetworkManager::GAME_COMMUNICATE;
				mes.m[1] = (char) SET_PLAYER;

				//Headers are 2 bytes, id is 4 bytes
				mes.len = 2 + 4;

				memcpy(mes.m + 2, &(playerid), sizeof(playerid));
				net->send_client_message(mes, _endpoint);
				delete [] mes.m;
				mes.m = NULL;
				break;
		}
	}
}

void GameManager::request_add_player(NetworkManager* net){
	if(net->mode == NetworkManager::SERVER){
		logError("Adding player is not a server function");
		return;
	}
	NetworkManager::Message m;
	m.m = new char[NetworkManager::PACKET_SIZE];

	m.m[0] = (char) NetworkManager::GAME_COMMUNICATE;
	m.m[1] = (char) ADD_PLAYER;

	//2 bytes for the headers (nothing else)
	m.len = 2;

	net->send_server_message(m);

	delete [] m.m;
	m.m = NULL;
}
