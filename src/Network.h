#ifndef _NETWORK_H
#define _NETWORK_H

#define ASIO_STANDALONE
#include <asio.hpp>
#include <string>
#include "Graphics.h"

struct Packet{

};

class Network{
	public:
		Network();
		~Network();

		void create_local_server(int port);

		//Warning: blocks thread
		void recieve_client_message(int size);


		void connect_to_server(std::string hostname);
		void send_server_message(char* message, int len);
		char* recieve_server_message();

		static const int PACKET_SIZE;
		static const std::string SERVICE_NAME;
	private:

		asio::ip::udp::socket* server;
		asio::ip::udp::socket* client;
		asio::ip::udp::endpoint server_loc;
		asio::io_service io_service;


		std::vector<std::string> messages;
};

#endif
