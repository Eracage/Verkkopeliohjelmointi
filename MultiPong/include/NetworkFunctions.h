#pragma once
#ifndef NETWORKFUCNTIONS_H
#define	NETWORKFUCNTIONS_H

#include <SFML\Network.hpp>
#include <World.h>

struct Client
{
	sf::IpAddress address;
	sf::Uint16 port;
	sf::Uint32 ID;
	std::string name;
	sf::Uint32 ping;
	bool dead = false;
};

// 0
sf::Packet & MakeConnectPacket(sf::Packet & packet, std::string & name);
// 4
sf::Packet & MakePlayerUpdatePacket(sf::Packet & packet, WorldPlayer player);
// 5
sf::Packet & MakeWorldPacket(sf::Packet & packet, World & world, sf::Int32 ping, sf::Int32 runtime);
// 7
sf::Packet & MakeReceivedPacket(sf::Packet & packet, sf::Uint32 ID, sf::Int32 runtime);

#endif
