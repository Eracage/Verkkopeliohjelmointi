#include <Game.h>
#include <util.h>
#include <iostream>

Game::Game(sf::Font & fnt, World & w)
	: view(sf::FloatRect(-400, -400, 800, 800)),
	font(fnt),
	world(w)
{
	//paddles.push_back(Paddle(font));
	view.setViewport(sf::FloatRect(0, 0, 0.625f, 1));
}


Game::~Game()
{
}

void Game::Update(sf::RenderWindow & window)
{
	window.setView(view);

	auto pInfos = world.GetPlayerInfo();

	playerCount = pInfos.size();

	sf::Color wallColor = sf::Color::Blue;

	while (players.size() < playerCount)
	{
		players.push_back(Paddle(font));
		walls.push_back(Paddle(font));
	}

	for (int i = 0; i < playerCount; i++)
	{
		players[i].SetPoints(pInfos[i].PaddleRight, pInfos[i].PaddleLeft);
		players[i].SetColor(pInfos[i].PlayerColor);

		if (pInfos[i].ID == ID)
			view.setRotation(pInfos[i].degrees - 90);

		walls[i].SetColor(wallColor);
		walls[i].SetPoints(pInfos[(i + 1) % playerCount].WallRight, pInfos[i].WallLeft);
	}

	auto bInfos = world.GetBallInfo();

	ballCount = world.balls.size();

	while (balls.size() < ballCount)
	{
		balls.push_back(Paddle(font));
	}

	for (int i = 0; i < ballCount; i++)
	{
		balls[i].SetPoints(bInfos[i].Position, bInfos[i].Position);
		balls[i].SetColor(bInfos[i].BallColor);
		balls[i].SetWidth(bInfos[i].radius);
	}

	if (window.hasFocus() && world.players.size() > ID)
	{
		int dir =
			(sf::Keyboard::isKeyPressed(sf::Keyboard::S) |
				sf::Keyboard::isKeyPressed(sf::Keyboard::A) |
				sf::Keyboard::isKeyPressed(sf::Keyboard::Down) |
				sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) -
			(sf::Keyboard::isKeyPressed(sf::Keyboard::W) |
				sf::Keyboard::isKeyPressed(sf::Keyboard::D) |
				sf::Keyboard::isKeyPressed(sf::Keyboard::Up) |
				sf::Keyboard::isKeyPressed(sf::Keyboard::Right));

		world.players[ID].pos[1] = dir * 0.001f;
		world.players[ID].pos[0] = world.PlayerPos(ID, world.runTime);
		world.players[ID].pos[1] = 0;
		world.players[ID].time = world.runTime;
		//std::cout << world.players[ID].pos[0] << " : " << world.players[ID].pos[1] << std::endl;
	}
}

void Game::Draw(sf::RenderWindow & window)
{
	window.setView(view);

	for (size_t i = 0; i < playerCount; i++)
	{
		walls[i].Draw(window);
	}
	for (size_t i = 0; i < playerCount; i++)
	{
		players[i].Draw(window);
	}
	for (size_t i = 0; i < ballCount; i++)
	{
		balls[i].Draw(window);
	}
}
