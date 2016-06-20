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
	send_server_message(m.m.get(), m.len);
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
	mes.m = std::shared_ptr<char>(new char[PACKET_SIZE]);
	size_t len = socket->receive_from(asio::buffer(mes.m.get(), PACKET_SIZE), server_loc);

	std::cout<<"Server sent:"<<mes.m.get()<<'\n';

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

void NetworkManager::broadCastMessage(Message m){
	broadCastMessage(m.m.get(), m.len);
}

void NetworkManager::broadCastMessage(char* message, unsigned int size){
	for(udp::endpoint e : clients){
		asio::error_code error;
		socket->send_to(asio::buffer(message, size), e, 0, error);
	}
}

void yelp(const asio::error_code& error, std::size_t bytes){
	std::cout<<"Yelp!\n";
}

void NetworkManager::handleMessages(const asio::error_code &error,
		std::size_t bytes){
	std::cout<<"Message: "<<std::string(_buffer, bytes)<<"\n";
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
	io_service.reset();
	listen();
	threadState = true;
	listenThread = new std::thread(&NetworkManager::poll_handlers, this);
}

void NetworkManager::pass_handler(void (*handler)(const asio::error_code& error, std::size_t bytes)){
	socket->async_receive_from(asio::buffer(_buffer, PACKET_SIZE), _endpoint, 0, handler);
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
