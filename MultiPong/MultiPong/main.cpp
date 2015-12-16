#define WIN32_LEAN_AND_MEAN

#include <vector>
#include <thread>
#include <regex>
#include <Regexer.h>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <Paddle.h>
#include <Game.h>
#include <Login.h>
#include <util.h>

#include <NetworkFunctions.h>
#include <Client.h>

#include <iostream>

int main(int argc, char* argv[])
{
	int frames = 0;
	for (size_t i = 0; i < argc; i++)
	{
		std::cout << argv[i] << std::endl;
	}
	std::cout << std::endl;

	World world;

	Client client(world);

	std::string ipString = "localhost";
	std::string portString = "27012";
	std::string ownPortString = "";
	std::string ownName = "Client";

	switch (argc)
	{
	default:
	case 4:
		ownPortString = argv[3];
	case 3:
		portString = argv[2];
	case 2:
		ipString = argv[1];
	case 1:
		//ownName = argv[0];
	case 0:
		break;
	}

	if (!client.Connect(ipString, portString, ownPortString, ownName))
	{
		return -1;
	}

	std::cout << "Connected to " << client.server.address << ":" << client.server.port << std::endl;

	auto start = std::chrono::high_resolution_clock::now();

	sf::Int32 dt = 10;
	sf::Int32 runtime = 0;

	sf::RenderWindow window(sf::VideoMode(1280, 800), "MultiPong by Eracage!", sf::Style::Titlebar | sf::Style::Close);

	sf::Font font;
	font.loadFromFile("comicbd.ttf");

	Game game(font, world);

	game.ID = client.ownData.ID;

	//bool loginActive = true;

	//Login login(font, loginActive, window);

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
				//login.Input(c);
			}
			default:
				break;
			}
		}


		for (size_t i = 0; client.Receive(runtime) && i < 10000; i++);

		while (world.runTime < runtime + client.ServerTimeDif + client.ownData.ping)
			world.Update(dt);


		auto dif = (std::chrono::high_resolution_clock::now() - start) - std::chrono::milliseconds(runtime + dt);
		std::this_thread::sleep_until(std::chrono::high_resolution_clock::now() - dif);
		//if (dif > std::chrono::milliseconds(dt))
		//{
			runtime += dt;

			game.Update(window);

			client.Update();

			window.clear();

			game.Draw(window);

			//login.Draw();

			window.display();
		//}
			frames++;

			if (runtime % 1000 == 0)
			{
				std::cout << runtime / 1000 << " : " << frames << std::endl;
				frames = 0;
			}


		//if ((std::chrono::high_resolution_clock::now() - start) >
		//	std::chrono::milliseconds(runtime + dt))
		//{
		//	runtime += dt;

		//	game.Update(window);

		//	client.Update();

		//	window.clear();

		//	game.Draw(window);

		//	//login.Draw();

		//	window.display();
		//}
	}

	return 0;
}
