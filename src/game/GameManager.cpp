#include "GameManager.h"
#include "game/GameObject.h"
#include "game/Player.h"
#include "game/Bullet.h"

GameManager::GameManager(){
	localPlayer = NULL;
	__buffer = new char[NetworkManager::PACKET_SIZE];
	curId = 1;
	gameThread = NULL;
}

bool GameManager::EndpointComparator::operator()(const asio::ip::udp::endpoint& e1,
		const asio::ip::udp::endpoint& e2) const {
	return e1.address().to_string().compare(e2.address().to_string()) > 0;
}

void GameManager::setLocalPlayer(int id){
	if(localPlayer != NULL){
	if(localPlayer->id == id){
		return;
	}
	}
	localPlayer = static_cast<Player*>(get_object(id));
	if(localPlayer == NULL){
		logError("Local player not found rip");
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

inline GameObject* GameManager::get_object(int id){
	for(GameObject* g : objects){
		if(g->id == id){
			return g;
		}
	}
	return NULL;
}

void GameManager::set_player_input(int id, NetworkManager::Message m){
	Player* p = static_cast<Player*>(get_object(id));
	if(p == NULL){
		logError("Player not found to set input");
	}

	p->getInput(m);
}

GameManager::~GameManager(){
	while(!objects.empty()){
		GameObject* o = objects.back();
		delete o;
		objects.pop_back();
	}

	if(gameThread != NULL){
		delete gameThread;
		gameThread = NULL;
	}

	gameThread = NULL;

	if(__buffer != NULL){
		delete [] __buffer;
		__buffer = NULL;
	}
}

void GameManager::startGame(){
	//Debugging purposes only currently
	status = GameManager::State::DURING;

	gameThread = new std::thread(&GameManager::manageGame, this);
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
	if(localPlayer != NULL){
		localPlayer->getInput();
	}
	while(SDL_PollEvent(&event)){

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
	buffer[0] = (char) NetworkManager::GAME_COMMUNICATE;
	buffer[1] = (char) UPDATE;
	for(GameObject* g : objects){

		//buffer+2 because 2 bytes are for headers
		NetworkManager::Message m = g->serialize(buffer+HEADER_SIZE);
		m.m = buffer;
		m.len += HEADER_SIZE;
		net->broadcastMessage(m);
	}

	delete [] buffer;
	buffer = NULL;
}

void GameManager::update_gamestate(NetworkManager::Message m){
	if(m.len <= 4){
		logError("Message is corrrupted");
		std::cout<<"Message size: "<<m.len<<'\n';
		return;
	}
	logError("Gamestate updated");
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
				std::cout<<"Id: "<<temp_id<<'\n';
				logError("Adding player");
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

void GameManager::game_handler(NetworkManager* net, const asio::error_code& error,
		std::size_t bytes){
	if(net->socket == NULL){
		logError("nothing has been initialized bro");
		return;
	}
	using namespace std::placeholders;

	net->socket->async_receive_from(
			asio::buffer(__buffer, NetworkManager::PACKET_SIZE),
			remote, 0,
			std::bind(&GameManager::game_handler, this, net, _1, _2));
	if(bytes <= 2){
		return;
	}
	if(__buffer[0] != NetworkManager::GAME_COMMUNICATE){
		return;
	}

	NetworkManager::Message m; //This message has been edited
	m.m = __buffer + HEADER_SIZE; //First byte is network stuff, 2 byte is game header
	m.len = bytes - HEADER_SIZE; //Cuz I added 2 above

	if(net->mode == NetworkManager::CLIENT){
		switch(__buffer[1]){
			case UPDATE:
				update_gamestate(m);
				break;
			case SET_PLAYER:
				{
					int id;
					memcpy(&id, __buffer+HEADER_SIZE, sizeof(id));
					setLocalPlayer(id);
				}
				break;
			case SET_PLAYER_INPUT:
				{
					int id;
					memcpy(&id, __buffer+HEADER_SIZE, sizeof(id));
					set_player_input(id, m);
				}
				break;
		}
	}
	if(net->mode == NetworkManager::SERVER){
		switch(__buffer[1]){
			case ADD_PLAYER:
				if(client_players.find(remote) == client_players.end())
				{
					if(client_players.size() != 0){
						std::cout<<"Address 1: "<<remote.address()<<'\n';
						std::cout<<"Address 2: "<<client_players.begin()->first.address()<<'\n';
						std::cout<<(remote==client_players.begin()->first)<<'\n';
					}
					logError("Adding Player");
					//Make and Add the player
					Player* p = new Player(this);
					p->unserialize(m);
					p->id = curId++;
					addObject(p);
					client_players.insert(
							std::pair<asio::ip::udp::endpoint, int>
							(remote, p->id));
				}
				{
					//Respond to request
					uint32_t id = client_players[remote];
					//Send the player informtation back
					NetworkManager::Message response;
					response.m = new char[NetworkManager::PACKET_SIZE];
					
					//2 because there are 2 headers ("GAMECOMM and SET_PLAYER
					//4 because the id is 4 bytes long
					response.len = HEADER_SIZE + 4;
					response.m[0] = (char) NetworkManager::GAME_COMMUNICATE;
					response.m[1] = (char) SET_PLAYER;
					memcpy(response.m + HEADER_SIZE, &(id), sizeof(id));
					net->send_client_message(response, remote);
					delete [] response.m;
					response.m = NULL;
				}
				break;
			case GET_PLAYER:
				{
					logError("Someone's callin a deprecated method...");
					//Deprecated (should not be called)
					int playerid = 0;
					if(client_players.count(remote) > 0){
						playerid = client_players[remote];
					}
					else{
						break;
					}
					NetworkManager::Message mes;
					mes.m = new char[NetworkManager::PACKET_SIZE];
					mes.m[0] = (char) NetworkManager::GAME_COMMUNICATE;
					mes.m[1] = (char) SET_PLAYER;

					//Headers are 2 bytes, id is 4 bytes
					mes.len = HEADER_SIZE + sizeof(playerid);

					memcpy(mes.m + HEADER_SIZE, &(playerid), sizeof(playerid));
					net->send_client_message(mes, remote);
					delete [] mes.m;
					mes.m = NULL;
				}
				break;

			//Assume that it's a player command
			case SET_PLAYER_INPUT:
				{
					Player* p = NULL;
					//Find the guy (Probably should move away from this at some
					//point
					int id = client_players[remote];
					for(GameObject* g : objects){
						if(g->id == id){
							p = static_cast<Player*>(g);
							break;
						}
					}

					if(p == NULL){
						logError("Not a player...");
						break;
					}

					p->getInput(m);
				}
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

	Player p(this);
	p.serialize(m.m + HEADER_SIZE);

	//2 bytes for the headers, more bytes for players
	m.len = HEADER_SIZE + Player::PLAYERDATA_SIZE;

	net->send_server_message(m);

	delete [] m.m;
	m.m = NULL;
}

void GameManager::join_server(NetworkManager* net){
	logError("Joining server");
	NetworkManager::Message join_mes;
	join_mes.m = new char[NetworkManager::PACKET_SIZE];
	join_mes.len = 1;
	join_mes.m[0] = (char) NetworkManager::REQ_CONNECT;

	net->send_server_message(join_mes);

	delete [] join_mes.m;
	join_mes.m = NULL;
}

void GameManager::send_server_player_input(NetworkManager* net){
	if(net->socket == NULL){
		logError("Bruh");
		return;
	}
	if(localPlayer == NULL){
		logError("No local player found");
	}
	NetworkManager::Message m;
	m.m = new char[NetworkManager::PACKET_SIZE];
	m.len = HEADER_SIZE;
	m.m[0] = NetworkManager::GAME_COMMUNICATE;
	m.m[1] = GameManager::SET_PLAYER_INPUT;
	NetworkManager::Message inp = localPlayer->serializeInput(m.m+HEADER_SIZE);
	m.len += inp.len;

	net->send_server_message(m);

	delete [] m.m;
	m.m = NULL;
}
