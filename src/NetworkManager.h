#ifndef _NETWORKMANAGER_H
#define _NETWORKMANAGER_H

#define ASIO_STANDALONE
#include <asio.hpp>
#include <string>
#include <list>
#include <memory>
#include <thread>
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
		void broadCastMessage(Message m);
		void broadCastMessage(char* message, unsigned int len);
		//Warning: blocks thread
		void receive_client_message();
		void startClientMessageThreads();
		void pass_handler(void (*handler)(const asio::error_code& error, std::size_t bytes));


		//Client stuff
		void connect_to_server(std::string hostname);
		void send_server_message(std::string message);
		void send_server_message(Message m);
		void send_server_message(char* message, unsigned int len);
		Message receive_server_message();

		static const int PACKET_SIZE;
		static const std::string SERVICE_NAME;

		MODE mode;
	private:
		void poll_handlers();
		std::thread* listenThread;
		bool threadState = false;

		void listen();
		char* _buffer;


		//Debugging purposes only
		void handleMessages(const asio::error_code& error, std::size_t bytes);

		asio::ip::udp::socket* socket;

		//Server variables
		std::list<asio::ip::udp::endpoint> clients;
		asio::ip::udp::endpoint _endpoint;

		//Client variables
		asio::ip::udp::endpoint server_loc;

		asio::io_service io_service;
		std::list<Message> messages;
};

#endif
