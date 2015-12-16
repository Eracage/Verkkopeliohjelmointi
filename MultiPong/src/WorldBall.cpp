#include "WorldBall.h"



WorldBall::WorldBall()
{
}


WorldBall::~WorldBall()
{
}

sf::Packet & operator<<(sf::Packet & packet, const WorldBall & ball)
{
	for (int i = 0; i < 3; i++)
		packet << ball.pos[i].x << ball.pos[i].y;
	packet << ball.color << ball.time;
	return packet;
}

sf::Packet & operator>>(sf::Packet & packet, WorldBall & ball)
{
	for (int i = 0; i < 3; i++)
		packet >> ball.pos[i].x >> ball.pos[i].y;
	packet >> ball.color >> ball.time;
	return packet;
}
