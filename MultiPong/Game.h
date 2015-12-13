#pragma once

#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>
#include "Paddle.h"


class Game
{
public:
	Game(sf::Font & font);
	~Game();

	void Update(sf::RenderWindow & window);
	void Draw(sf::RenderWindow& window);

	sf::View view;

	std::vector<Paddle> map;
	std::vector<Paddle> paddles;
	sf::Font & font;
};

