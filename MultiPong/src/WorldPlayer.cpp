#include "WorldPlayer.h"



WorldPlayer::WorldPlayer()
{
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
	packet >> player.ID >> player.color >> player.time >> player.joinTime >> player.leaveTime;
	for (int i = 2; i >= 0; i--)
		packet >> player.pos[i];
	return packet;
}
