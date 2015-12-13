#define WIN32_LEAN_AND_MEAN

#include <vector>
#include <thread>
#include <regex>

#include <SFML/Graphics.hpp>

#include "Paddle.h"
#include "Game.h"
#include "Login.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 800), "MultiPong by Eracage!", sf::Style::Titlebar | sf::Style::Close);

	sf::Font font;
	font.loadFromFile("comicbd.ttf");

	bool loginActive = true;

	Login login(font, loginActive, window);

	Game game(font);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::TextEntered:
			{
				sf::Uint32 c = event.text.unicode;
				login.Input(c);
			}
			default:
				break;
			}
		}

		login.Update();

		game.Update(window);

		window.clear();

		game.Draw(window);

		login.Draw();

		window.display();
	}

	return 0;
}
