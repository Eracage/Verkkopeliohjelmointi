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

#include <iostream>

int main(int argc, char* argv[])
{
	Client server;
	Client remote;
	sf::Int64 PingSum = 0;
	sf::Int32 PingCount = 1;
	sf::Int32 Ping = 0;
	sf::Int32 ServerTimeDif = 0;

	sf::Int32 LastWorldRuntime = 0;
	sf::Int32 ownWorldRuntime = 0;

	server.name = "ClientName";

	std::string portString = regexArgPort(argc, argv, 0, "12345");
	std::string ipString = regexArgIP(argc, argv, 0, "localhost");
	std::string ownPortString = regexArgPort(argc, argv, 2, "27001");

	server.port = std::stoi(portString);
	server.address = sf::IpAddress(ipString);

	auto start = std::chrono::high_resolution_clock::now();

	sf::Int32 dt = 10;
	sf::Int32 runtime = 0;

	sf::RenderWindow window(sf::VideoMode(1280, 800), "MultiPong by Eracage!", sf::Style::Titlebar | sf::Style::Close);

	sf::Font font;
	font.loadFromFile("comicbd.ttf");

	sf::UdpSocket socket;
	socket.setBlocking(false);


	//if (socket.bind(std::stoi(ownPortString)) != sf::Socket::Done)
		if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
			std::cout << "error: socket.bind" << std::endl;

	sf::Packet packet;

	MakeConnectPacket(packet, server.name);

	if (socket.send(packet, server.address, server.port) != sf::Socket::Done)
		std::cout << "error: socket.send" << std::endl;

	// useful delay
	World world;
	World ownWorld;

	Game game(font, world, ownWorld);

	int c = 1;
	//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	packet.clear();
	while (socket.receive(packet, remote.address, remote.port) == sf::Socket::NotReady)
	{
		if (c % 50)
		{
			std::cout << "Connecting..." << std::endl;
			if (socket.send(packet, server.address, server.port) != sf::Socket::Done)
				std::cout << "error: socket.send" << std::endl;
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		c++;
	}

	sf::Uint32 pType;
	packet >> pType;

	switch (pType)
	{
	case 7:
	{
		sf::Uint32 ID;
		sf::Int32 rTime;

		packet >> rTime;
		packet >> ID;
		packet.clear();

		while ((std::chrono::high_resolution_clock::now() - start) >
			std::chrono::milliseconds(runtime + dt))
		{
			runtime += dt;
		}

		ServerTimeDif = rTime - runtime;
		server.ID = ID;
	}
		break;
	default:
		std::cout << "error: unexpected packet type" << std::endl;
		break;
	}

	game.ID = server.ID;


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

		int m = 0;
		const int maxM = 10000;

		packet.clear();
		while (socket.receive(packet, server.address, server.port) != sf::Socket::NotReady
			&& m < maxM)
		{
			packet >> pType;
			switch (pType)
			{
			case 5:
			{
				sf::Int32 runtime;
				sf::Uint32 bSize;
				sf::Uint32 pSize;

				packet >> runtime;

				if (LastWorldRuntime > runtime)
					break;

				packet >> server.ping;

				packet >> bSize;
				for (int i = 0; i < bSize; i++)
				{
					if (i < world.balls.size())
					{
						packet >> world.balls[i];
					}
					else
					{
						WorldBall b;
						packet >> b;
						world.balls.push_back(b);
						ownWorld.balls.push_back(b);
					}
				}

				packet >> pSize;
				for (int i = 0; i < pSize; i++)
				{
					if (i < world.players.size())
					{
					packet >> world.players[i];
					}
					else
					{
						WorldPlayer p;
						packet >> p;
						world.players.push_back(p);
						ownWorld.players.push_back(p);
					}
				}
				ownWorldRuntime = LastWorldRuntime = runtime;

				PingSum += server.ping;
				PingCount++;
				packet.clear();
			}
				break;
			case 7:
				packet.clear();
				break;
			default:
				std::cout << "error: unexpected packet type" << std::endl;
				break;
			}
			m++;
		}

		//login.Update();


		while ((std::chrono::high_resolution_clock::now() - start) >
			std::chrono::milliseconds(runtime + dt))
		{
			runtime += dt;

			sf::Int32 avPing;

			avPing = PingSum / PingCount / 2;
			
			while (avPing > server.ping)
			{
				PingCount = PingCount * 3 / 2;
				avPing = PingSum / PingCount / 2;
			}

			sf::Int32 serverTime = runtime + ServerTimeDif + avPing;
			while (ownWorldRuntime < serverTime)
			{
				ownWorldRuntime += dt;
				ownWorld.Update(dt);
			}

			game.Update(window);
		}

		packet.clear();
		MakePlayerUpdatePacket(packet, ownWorld.players[server.ID]);
		socket.send(packet, server.address, server.port);
		packet.clear();

		window.clear();

		game.Draw(window);

		//login.Draw();

		window.display();
	}

	return 0;
}
