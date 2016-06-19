#ifndef _NETWORKMANAGER_H
#define _NETWORKMANAGER_H

#define ASIO_STANDALONE
#include <asio.hpp>
#include <string>
#include <list>
#include "Graphics.h"

class NetworkManager{
	public:
		struct Message{
			char* m;
			size_t len;
		};
		enum MODE{
			SERVER, CLIENT
		};
		NetworkManager(MODE mode);
		~NetworkManager();

		//Server stuff
		void create_local_server(int port);
		void broadCastMessage(char* message, unsigned int len);

		//Warning: blocks thread
		void receive_client_message(int size);

		//Client stuff
		void connect_to_server(std::string hostname);
		void send_server_message(Message m);
		void send_server_message(char* message, unsigned int len);
		Message receive_server_message();

		static const int PACKET_SIZE;
		static const std::string SERVICE_NAME;
	private:

		MODE mode;
		asio::ip::udp::socket* socket;

		//Server variables
		std::list<asio::ip::udp::endpoint> clients;

		//Client variables
		asio::ip::udp::endpoint server_loc;

		asio::io_service io_service;

		std::vector<std::string> messages;
};

#endif
