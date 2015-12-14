#include <NetworkFunctions.h>

sf::Packet & MakeConnectPacket(sf::Packet & packet, std::string & name)
{
	sf::Uint32 pType = 0;
	packet << pType;
	packet << name;
	return packet;
}

sf::Packet & MakePlayerUpdatePacket(sf::Packet & packet, WorldPlayer player)
{
	sf::Uint32 pType = 4;
	packet << pType;
	packet << player;
	return packet;
}

sf::Packet & MakeWorldPacket(sf::Packet & packet, World & world, sf::Int32 ping, sf::Int32 runtime)
{
	sf::Uint32 pType = 5;
	sf::Uint32 pSize = world.players.size();
	sf::Uint32 bSize = world.balls.size();

	packet << pType;

	packet << runtime;

	packet << ping;

	packet << bSize;
	for (int i = 0; i < world.balls.size(); i++)
	{
		packet << world.balls[i];
	}

	packet << pSize;
	for (int i = 0; i < world.players.size(); i++)
	{
		packet << world.players[i];
	}

	return packet;
}

sf::Packet & MakeReceivedPacket(sf::Packet & packet, sf::Uint32 ID, sf::Int32 runtime)
{
	sf::Uint32 pType = 7;
	packet << pType;
	packet << runtime;
	packet << ID;
	return packet;
}
