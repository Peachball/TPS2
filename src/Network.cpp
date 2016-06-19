#include "Network.h"

using asio::ip::udp;

const std::string Network::SERVICE_NAME = "TPS2";
const int Network::PACKET_SIZE = 128;

Network::Network(){
	server = NULL;

}

void Network::connect_to_server(std::string hostname){
	if(client != NULL){
		logError("Client already initialized");
		client->close();
		delete client;
		client = NULL;
	}

	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), hostname, SERVICE_NAME);
	server_loc = *resolver.resolve(query);
	client = new udp::socket(io_service);

	client->open(udp::v4());
}

void Network::send_server_message(char * message, int len){
	if(client == NULL){
		logError("server location was deallocated, or the client wasn't"
				"created");
		return;
	}
	char buf[PACKET_SIZE];
	for(int i = 0; i < len; i++){
		buf[i] = message[i];
	}
	client->send_to(asio::buffer(buf), server_loc);
}

char* Network::recieve_server_message(){
	char buffer[PACKET_SIZE];

}

void Network::create_local_server(int port){
	if(server != NULL){
		logError("Server already exists, and will be replaced");

		server->close();
		delete server;
		server = NULL;
	}
	server = new udp::socket(io_service, udp::endpoint(udp::v4(), port));
}

void Network::recieve_client_message(int size){
	if(server == NULL){
		logError("Server not initialized yet");
		return;
	}
	char recv_buf[PACKET_SIZE];
	udp::endpoint remote_endpoint;
	asio::error_code error;
	server->receive_from(asio::buffer(recv_buf), remote_endpoint, 0, error);

	if(error && error != asio::error::message_size){
		throw asio::system_error(error);
	}
}

Network::~Network(){
	if(client != NULL){
		delete client;
	}

	if(server != NULL){
		delete server;
	}
}
