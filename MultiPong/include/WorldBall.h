#pragma once
#ifndef WORLDBALL_H
#define WORLDBALL_H

#include <SFML\Network.hpp>


class WorldBall
{
public:
	WorldBall();
	~WorldBall();

	sf::Vector2f pos[3];
	sf::Uint32 color;
	int time;
};

sf::Packet& operator <<(sf::Packet& packet, const WorldBall& ball);

sf::Packet& operator >>(sf::Packet& packet, WorldBall& ball);

#endif
