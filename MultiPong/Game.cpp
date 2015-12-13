#include "Game.h"



Game::Game(sf::Font & fnt)
	: view(sf::FloatRect(-400, -400, 800, 800)),
	font(fnt)
{
	paddles.push_back(Paddle(font));
	view.setViewport(sf::FloatRect(0, 0, 0.625f, 1));
}


Game::~Game()
{
}

void Game::Update(sf::RenderWindow & window)
{
	window.setView(view);
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
	paddles[0].SetPoints(sf::Vector2f(0, 0), mousePos);
}

void Game::Draw(sf::RenderWindow & window)
{
	window.setView(view);

	for each (Paddle wall in map)
	{
		wall.Draw(window);
	}
	for each (Paddle paddle in paddles)
	{
		paddle.Draw(window);
	}
}
