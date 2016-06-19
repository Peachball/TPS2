#include "NetworkManager.h"

using asio::ip::udp;

const std::string NetworkManager::SERVICE_NAME = "2000";
const int NetworkManager::PACKET_SIZE = 128;

NetworkManager::NetworkManager(MODE mode){
	socket = NULL;

	this->mode = mode;
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

void NetworkManager::send_server_message(char* message, unsigned int len){
	if(mode == SERVER){
		logError("Currently running as server, not client");
		return;
	}
	if(socket == NULL){
		logError("Socket was not initialized");
	}
	std::cout<<"Sending server message: "<<message<<'\n';
	std::cout<<"Server address: "<<server_loc.address()<<'\n';
	socket->send_to(asio::buffer(message, len), server_loc);
	std::cout<<"Sent!\n";
}

NetworkManager::Message NetworkManager::receive_server_message(){
	Message mes;
	if(mode == SERVER){
		logError("Currently running as server, not client");
		return mes;
	}
	char buffer[PACKET_SIZE];
	size_t len = socket->receive_from(asio::buffer(buffer), server_loc);

	std::cout<<"Server sent:"<<buffer<<'\n';

	mes.m = buffer;
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

void NetworkManager::broadCastMessage(char* message, unsigned int size){
	for(udp::endpoint e : clients){
		asio::error_code error;
		socket->send_to(asio::buffer(message, size), e, 0, error);
	}
}

void NetworkManager::listen(){
	while(threadState){
		receive_client_message();
	}
}

void NetworkManager::startClientMessageThreads(){
	threadState = true;
	listenThread = new std::thread(&NetworkManager::listen, this);
}

NetworkManager::~NetworkManager(){
	io_service.stop();
	threadState = false;
	if(listenThread != NULL){
		listenThread->join();
		delete listenThread;
	}
	if(socket != NULL){
		socket->close();
		delete socket;
	}
}
