#pragma once

#include "util.h"

class WorldPlayer
{
public:
	WorldPlayer();
	~WorldPlayer();

	float centerAngle[3];
	float opening[3];

	sf::Vector2f pos[3];

	sf::Uint32 ID;
	sf::Uint32 color;
	float time;
};

