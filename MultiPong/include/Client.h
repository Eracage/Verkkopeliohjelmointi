#pragma once
#ifndef	CLIENT_H
#define CLIENT_H

#include <NetworkFunctions.h>

class Client
{
public:
	Client(World & world);
	~Client();

	bool Connect(std::string ip, std::string port, std::string ownPort = "", std::string ownName = "Client");

	bool Receive(sf::Int32 runtime);

	void Update();

	World & world;

	sf::UdpSocket socket;
	sf::Packet packet;
	sf::Socket::Status status;
	sf::Uint32 pType;

	sf::Int32 ServerTimeDif;
	sf::Int32 sWorldRunTime;

	ConnectionData ownData;
	ConnectionData server;
	ConnectionData remote;
};

#endif
