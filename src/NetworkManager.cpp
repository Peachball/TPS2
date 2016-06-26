#include "NetworkManager.h"

using asio::ip::udp;

const std::string NetworkManager::SERVICE_NAME = "2000";
const int NetworkManager::PACKET_SIZE = 128;

NetworkManager::NetworkManager(MODE mode){
	socket = NULL;
	listenThread = NULL;

	this->mode = mode;
	_buffer = new char[PACKET_SIZE];
}


void NetworkManager::connect_to_server(std::string hostname){
	if(mode == SERVER){
		logError("Currently running as server, not client");
		return;
	}
	if(socket != NULL){
		logError("This function was called twice");
		return;
	}
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), hostname, SERVICE_NAME);
	server_loc = *resolver.resolve(query);
	socket = new udp::socket(io_service);
	socket->open(udp::v4());
}

void NetworkManager::send_server_message(std::string message){
	send_server_message(&message[0], message.size());
}

void NetworkManager::send_server_message(Message m){
	send_server_message(m.m, m.len);
}

void NetworkManager::send_server_message(char* message, int len){
	if(mode == SERVER){
		logError("Currently running as server, not client");
		return;
	}
	if(socket == NULL){
		logError("Socket was not initialized");
	}
	socket->send_to(asio::buffer(message, len), server_loc);
}

NetworkManager::Message NetworkManager::receive_server_message(char* buffer){
	Message mes;
	if(mode == SERVER){
		logError("Currently running as server, not client");
		return mes;
	}
	mes.m = buffer;
	size_t len = socket->receive_from(asio::buffer(mes.m, PACKET_SIZE), server_loc);

	std::cout<<"Server sent:"<<mes.m<<'\n';

	mes.len = len;
	return mes;
}


void NetworkManager::create_local_server(int port){
	if(mode == CLIENT){
		logError("Currently running as server, not client");
		return;
	}
	if(socket != NULL){
		logError("This function was run twice");
		return;
	}
	socket = new udp::socket(io_service, udp::endpoint(udp::v4(), port));
}

void NetworkManager::receive_client_message(){
	if(mode == CLIENT){
		logError("Currently running as server, not client");
		return;
	}
	if(socket == NULL){
		logError("Socket not initialized");
		return;
	}
	char recv_buf[PACKET_SIZE];
	udp::endpoint remote_endpoint;
	asio::error_code error;
	std::cout<<"Waiting for messages...\n";
	socket->receive_from(asio::buffer(recv_buf), remote_endpoint, 0, error);

	std::cout<<"Client Sent Message:"<<recv_buf<<'\n';
	if(error && error != asio::error::message_size){
		throw asio::system_error(error);
	}
}

void NetworkManager::send_client_message(NetworkManager::Message m, udp::endpoint client){
	send_client_message(m.m, m.len, client);
}

void NetworkManager::send_client_message(char* message, int len,
		udp::endpoint client){
	asio::error_code error;
	socket->send_to(asio::buffer(message, len), client, 0, error);
}

void NetworkManager::broadcastMessage(Message m){
	broadcastMessage(m.m, m.len);
}

void NetworkManager::broadcastMessage(char* message, int size){
	for(udp::endpoint e : clients){
		asio::error_code error;
		socket->send_to(asio::buffer(message, size), e, 0, error);
	}
}

void NetworkManager::request_add_client(){
	Message m;
	m.m = new char[1];
	m.len = 1;
	m.m[0] = REQ_CONNECT;
	while(!connected_to_server){
		send_server_message(m);
	}
	delete [] m.m;
	m.m = NULL;
}

void yelp(const asio::error_code& error, std::size_t bytes){
	std::cout<<"Yelp!\n";
}

char NetworkManager::get_header(Message m){
	return m.m[0];
}

void NetworkManager::handleMessages(const asio::error_code &error,
		std::size_t bytes){
	Message m = {_buffer, bytes};
	if(mode == SERVER){
		//Originally planned to have more udp network actions, but there the
		//server really doesn't do much
		switch(get_header(m)){
			case REQ_CONNECT:
				add_client(_endpoint);

				{
					//Respond to connection request
					Message m;
					m.m = new char[1];
					m.len = 1;
					m.m[0] = ACCEPT_CONNECT;
					send_client_message(m, _endpoint);

					delete [] m.m;
				}
				break;
			case DISCONNECT:
				rem_client(_endpoint);
				break;
		}
	}
	if(mode == CLIENT){
		//Client does less atm
		switch(get_header(m)){
			case ACCEPT_CONNECT:
				connected_to_server = true;
				break;
		}
	}
	listen();
}

void NetworkManager::listen(){
	socket->async_receive_from(asio::buffer(_buffer, PACKET_SIZE), _endpoint, 0,
			std::bind(&NetworkManager::handleMessages, this,
				std::placeholders::_1,
				std::placeholders::_2));
}

void NetworkManager::poll_handlers(){
	while(threadState){
		io_service.poll();
	}
}

void NetworkManager::startClientMessageThreads(){
	threadState = true;
	listen();
	listenThread = new std::thread(&NetworkManager::poll_handlers, this);
}

void NetworkManager::add_client(udp::endpoint e){
	for(udp::endpoint cli : clients){
		if(cli == e){
			return;
		}
	}

	logError("Client accepted");
	//If e is not already added
	clients.push_back(e);
}

void NetworkManager::rem_client(udp::endpoint e){
	clients.remove(e);
}

void NetworkManager::reset(){
	io_service.reset();
}

NetworkManager::~NetworkManager(){
	io_service.stop();
	threadState = false;
	if(_buffer != NULL){
		delete [] _buffer;
		_buffer = NULL;
	}
	if(listenThread != NULL){
		listenThread->join();
		delete listenThread;
		listenThread = NULL;
	}
	if(socket != NULL){
		socket->close();
		delete socket;
		socket = NULL;
	}
}
