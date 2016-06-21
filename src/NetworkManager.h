#ifndef _NETWORKMANAGER_H
#define _NETWORKMANAGER_H

#define ASIO_STANDALONE

//msvs doesn't have c++11, so here we go:
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_TYPE_TRAITS

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

		//Possible network commands between server/client
		enum Command{
			DISCONNECT,
			//Client continuously sends this in intervals, if not recieved,
			//client is assumed to have dced
			HEARTBEAT,
			GAME_COMMUNICATE,

			//Connect as in connect to the game, not for the socket stuff
			REQ_CONNECT,
			ACCEPT_CONNECT
		};
		enum MODE{
			SERVER, CLIENT
		};

		NetworkManager(MODE mode);
		~NetworkManager();

		//Server stuff
		void create_local_server(int port);
		void broadcastMessage(Message m);
		void broadcastMessage(char* message, unsigned int len);
		//Warning: blocks thread
		void receive_client_message();
		void startClientMessageThreads();
		void add_client(asio::ip::udp::endpoint e);
		void rem_client(asio::ip::udp::endpoint e);
		void send_client_message(Message m, asio::ip::udp::endpoint client);
		void send_client_message(char* message, unsigned int len,
				asio::ip::udp::endpoint client);


		//Client stuff
		void connect_to_server(std::string hostname);
		void send_server_message(std::string message);
		void send_server_message(Message m);
		void send_server_message(char* message, unsigned int len);
		Message receive_server_message(char* buffer);

		static const int PACKET_SIZE;
		static const std::string SERVICE_NAME;

		MODE mode;

		asio::ip::udp::socket* socket;
		void listen();
		void reset();

	private:
		//Update all async tasks
		void poll_handlers();
		std::thread* listenThread;
		bool threadState = false;

		char* _buffer;

		//Handle connection stuff (ignore game connect stuff)
		void handleMessages(const asio::error_code& error, std::size_t bytes);
		char get_header(Message m);


		//Server variables
		std::list<asio::ip::udp::endpoint> clients;
		asio::ip::udp::endpoint _endpoint;

		//Client variables
		asio::ip::udp::endpoint server_loc;

		asio::io_service io_service;
		std::list<Message> messages;
};

#endif
