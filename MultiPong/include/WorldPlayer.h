#pragma once
#ifndef WORLDPLAYER_H
#define WORLDPLAYER_H


#include <SFML\Network.hpp>


class WorldPlayer
{
public:
	WorldPlayer();
	~WorldPlayer();

	float pos[3];

	sf::Uint32 ID;
	sf::Uint32 color;
	int time;
	float joinTime;
	float leaveTime;
};

sf::Packet& operator <<(sf::Packet& packet, const WorldPlayer& player);

sf::Packet& operator >>(sf::Packet& packet, WorldPlayer& player);

#endif
