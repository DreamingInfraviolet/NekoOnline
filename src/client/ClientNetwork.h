#pragma once

#include <stdexcept>
#include <string>
#define ASIO_MSVC 1
#define ASIO_STANDALONE 1
#include <asio.hpp>

class Network
{
public:
	Network()
	{

	}

	~Network()
	{

	}
};


/*
#include <Ws2tcpip.h>
#include <winsock.h>
class Network
{
	WSADATA wsaData;

#define CLOSE_SOCKET(x) closesocket(x)

	Network()
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw std::runtime_error("WSAStartup failed.");

		int port = 51234;

		addrinfo hints;
		

		memset(&hints, 0, sizeof hints); // make sure the struct is empty
		hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
		hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
		hints.ai_flags = AI_PASSIVE;     // fill in my IP for me


	
		addrinfo *servinfo;  // will point to the results
		getaddrinfo("192.158.0.1", "54323", &hints, &servinfo);

		int sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
		if (sock == -1)
			throw std::runtime_error("Unable to create socket");

		sockaddr_in server;
		server.sin_addr.s_addr = inet_addr("amareth.space"); //Might need IP?
		server.sin_family = AF_INET;
		server.sin_port = htons(port);

		//Connect to remote server
		if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
			throw std::runtime_error("Failed connecting to server");


		{
			int port = 51234;

			SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (listeningSocket !=0)
				throw std::runtime_error(std::string("Unable to create listening socket: ") + std::to_string(WSAGetLastError()));

			// Use a SOCKADDR_IN struct to fill in address information
			SOCKADDR_IN serverInfo;
			serverInfo.sin_family = AF_INET;
			serverInfo.sin_addr.s_addr = INADDR_ANY;	// Since this socket is listening for connections, any local address will do
			serverInfo.sin_port = htons(port);

			// Bind the socket to our local server address
			if (bind(listeningSocket, (LPSOCKADDR)&serverInfo, sizeof(struct sockaddr)) != 0)
				throw std::runtime_error(std::string("Unable to bind listening socket: ") + std::to_string(WSAGetLastError()));

			// Make the socket listen
			// Up to 10 connections may wait at any
			// one time to be accept()'ed

			if (listen(listeningSocket, 128) == SOCKET_ERROR)
				throw std::runtime_error(std::string("Unable to listen on socket: ") + std::to_string(WSAGetLastError()));

			// Wait for a client
			while (true)
			{
				SOCKET theClient;
				int s = sizeof(struct sockaddr_in);
				theClient = accept(listeningSocket, (sockaddr*)&serverInfo, &s);

				if (theClient == INVALID_SOCKET)
					throw std::runtime_error(std::string("Invalid client socket: ") + std::to_string(WSAGetLastError()));

				// Send and receive from the client, and finally,
				closesocket(theClient);
			}
			closesocket(listeningSocket);

		}
	}

	static Network* s_instance;
public:
	~Network()
	{
		WSACleanup();

		
	}

	

	

	Network* instance()
	{
		if (instance == nullptr)
			return s_instance = new Network();
		else
			return s_instance;
	}

	void destroy()
	{
		delete s_instance;
	}
};

Network* Network::s_instance = nullptr;
*/
