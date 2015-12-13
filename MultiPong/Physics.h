#pragma once

#include "util.h"

class PhysObject
{
public:
	sf::Uint32 ID;
	sf::Uint32 Color;
};

class PhysBall : PhysObject
{
public:
	sf::Vector2f pos;
	sf::Vector2f velocity;
	
};

class PhysWall
{

};

class PhysPaddle
{

};

class Physics
{
public:
	Physics();
	~Physics();
};

