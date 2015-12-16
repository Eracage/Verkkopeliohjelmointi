#pragma once
#ifndef NETWORKFUCNTIONS_H
#define	NETWORKFUCNTIONS_H

#include <SFML\Network.hpp>
#include <World.h>

struct ConnectionData
{
	sf::IpAddress address;
	sf::Uint16 port;
	sf::Uint32 ID;
	std::string name;
	sf::Uint32 ping;
	sf::Uint32 lastUpdate;
	bool maybeDisconnected = false;
	bool disconnected = false;
};

// 13
sf::Packet & MakeConnectPacket(sf::Packet & packet, std::string & name);
// 14
sf::Packet & MakePlayerUpdatePacket(sf::Packet & packet, WorldPlayer & player);
// 15
sf::Packet & MakeWorldPacket(sf::Packet & packet, World & world, sf::Int32 ping, sf::Int32 runtime);
// 16
sf::Packet & MakeDCPacket(sf::Packet & packet);
// 17
sf::Packet & MakeReceivedPacket(sf::Packet & packet, sf::Uint32 ID, sf::Int32 runtime);

#endif
