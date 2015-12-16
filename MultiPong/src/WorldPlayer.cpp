#include "WorldPlayer.h"
#include <SFML\Graphics.hpp>



WorldPlayer::WorldPlayer()
{
	color = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255).toInteger();
	ID = -1;
	joinTime = 0;
	time = 0;
	leaveTime = 0;
	pos[0] = 0.5f;
	pos[1] = 0.0f;
	pos[2] = 0.0f;
}


WorldPlayer::~WorldPlayer()
{
}

sf::Packet & operator<<(sf::Packet & packet, const WorldPlayer & player)
{
	for (int i = 0; i < 3; i++)
		packet << player.pos[i];
	packet << player.ID << player.color << player.time << player.joinTime << player.leaveTime;
	return packet;
}

sf::Packet & operator>>(sf::Packet & packet, WorldPlayer & player)
{
	for (int i = 0; i < 3; i++)
		packet >> player.pos[i];
	packet >> player.ID >> player.color >> player.time >> player.joinTime >> player.leaveTime;
	return packet;
}
