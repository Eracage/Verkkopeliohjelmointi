#pragma once
#ifndef	SERVER_H
#define SERVER_H

#include <NetworkFunctions.h>

class Server
{
public:
	Server(World & world);
	~Server();

	bool Bind(std::string port);

	bool Receive(sf::Int32 runtime);

	void Update(sf::Int32 runtime);

	void MakeDirty();

	World & world;

	sf::UdpSocket socket;
	sf::Packet packet;
	sf::Socket::Status status;
	sf::Uint32 pType;

	sf::Uint32 connectedClients;
	sf::Uint32 disconnectedClients;

	std::vector<ConnectionData> clients;
	ConnectionData remote;
	sf::Uint16 port;
};

#endif
