#pragma once
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>
#include <World.h>
#include <Paddle.h>


class Game
{
public:
	Game(sf::Font & font, World & world);
	~Game();

	void Update(sf::RenderWindow & window);
	void Draw(sf::RenderWindow& window);

	sf::View view;

	World & world;

	int playerCount;
	int ballCount;

	std::vector<Paddle> walls;
	std::vector<Paddle> players;
	std::vector<Paddle> balls;
	sf::Font & font;
	sf::Uint32 ID;
};

#endif
