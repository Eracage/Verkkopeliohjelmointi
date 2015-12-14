#include <Game.h>
#include <util.h>

bool serverWorld = true;

Game::Game(sf::Font & fnt, World & w1, World & ow)
	: view(sf::FloatRect(-400, -400, 800, 800)),
	font(fnt),
	world(w1),
	ownWorld(ow)
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

	World & w = serverWorld ? world : ownWorld;

	int playersAlive = 0;
	for (int i = 0; i < w.players.size(); i++)
	{
		if (w.players[i].leaveTime < 1)
		{
			playersAlive++;
		}
	}

	if (playersAlive > 1)
	{

		while (players.size() < playersAlive)
		{
			players.push_back(Paddle(font));
			walls.push_back(Paddle(font));
		}
		while (balls.size() < w.balls.size())
		{
			balls.push_back(Paddle(font));
		}

		float degrees = 360;

		float perPlayer = degrees / playersAlive;

		float playerWall = 1 / playersAlive;

		float playerArea = 1.0f - playerWall;

		float hlfp = 0.125f;

		sf::Vector2f start;
		sf::Vector2f end;

		sf::Vector2f pStart;
		sf::Vector2f pEnd;

		sf::Color wallColor = sf::Color::White;

		int size = w.players.size();

		// gotta do 1 extra
		for (int i = 0; i <= size; i++)
		{
			start = CirclePos(perPlayer * i + perPlayer * playerArea);
			// gotta be here
			walls[i%size].SetPoints(end, start);
			// in between
			end = CirclePos(perPlayer * i + perPlayer * playerArea);

			pStart = start + (end - start) * (w.players[i%size].pos[0] + hlfp);
			pEnd = start + (end - start) * (w.players[i%size].pos[0] - hlfp);

			players[i%size].SetPoints(pStart, pEnd);
			players[i%size].SetColor(sf::Color(w.players[i%size].color));

			walls[i%size].SetColor(wallColor);
		}

		size = balls.size();

		for (int i = 0; i < size; i++)
		{
			balls[i].SetPoints(w.balls[i].pos[0], w.balls[i].pos[0]);
			balls[i].SetColor(sf::Color(w.balls[i].color));
		}
	}


	//sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	//sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
	//paddles[0].SetPoints(sf::Vector2f(0, 0), mousePos);
}

void Game::Draw(sf::RenderWindow & window)
{
	window.setView(view);

	for each (Paddle wall in walls)
	{
		wall.Draw(window);
	}
	for each (Paddle player in players)
	{
		player.Draw(window);
	}
	for each (Paddle ball in balls)
	{
		ball.Draw(window);
	}
}
