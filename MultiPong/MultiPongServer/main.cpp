#define WIN32_LEAN_AND_MEAN

#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <World.h>
#include <iostream>
#include <regex>

#include <Regexer.h>
#include <NetworkFunctions.h>

int main(int argc, char* argv[])
{
	std::vector<Client> clients;
	Client client;

	std::string portString = regexArgPort(argc, argv, 0, "12345");

	auto start = std::chrono::high_resolution_clock::now();

	sf::Int32 dt = 10;
	sf::Int32 runtime = 0;
	
	//sf::RenderWindow window(sf::VideoMode(1280, 800), "MultiPong Server by Eracage!", sf::Style::Titlebar | sf::Style::Close);

	sf::Font font;
	font.loadFromFile("comicbd.ttf");

	sf::UdpSocket socket;
	socket.setBlocking(false);

	sf::Packet packet;

	sf::Uint32 pType;

	if (socket.bind(std::stoi(portString)) != sf::Socket::Done)
		std::cout << "error";

	World world;

	while (1)//window.isOpen())
	{
		sf::Event event;
		//while (window.pollEvent(event))
		//{
		//	switch (event.type)
		//	{
		//	case sf::Event::Closed:
		//		window.close();
		//		break;
		//	case sf::Event::TextEntered:
		//	{
		//		sf::Uint32 c = event.text.unicode;
		//	}
		//	default:
		//		break;
		//	}
		//}

		int m = 0;
		const int maxM = 10000;

		// Network Receive
		packet.clear();
		while (socket.receive(packet, client.address, client.port) != sf::Socket::NotReady
			&& m < maxM)
		{
			sf::Uint32 pType;
			packet >> pType;
			switch (pType)
			{
			case 0:
			{
				packet >> client.name;
				client.ID = clients.size();
				std::cout << client.name << " ID: " << client.ID << " Connecting...";

				bool found = false;
				for (int i = 0; i < clients.size(); i++)
				{
					if (client.ID == clients[i].ID &&
						client.address == clients[i].address &&
						client.port == clients[i].port)
						found = true;
				}
				if (!found)
				{
					clients.push_back(Client(client));
					world.AddPlayer(runtime);
				}

				packet.clear();
				MakeReceivedPacket(packet,client.ID,runtime);
				socket.send(packet, client.address, client.port);
				packet.clear();
			}
				break;
			case 4:
			{
				WorldPlayer player;
				packet >> player;

				if (player.ID >= clients.size())
				{
					std::cout << "error: player.ID >= clients.size()" << std::endl;
					break;
				}
				if (clients[player.ID].address != client.address)
				{
					std::cout << "error: clients[player.ID].address != client.address" << std::endl;
					break;
				}
				world.UpdatePlayer(player);
				packet.clear();
			}
				break;
			case 7:
			{
				sf::Uint32 ID;
				sf::Int32 rTime;

				packet >> rTime;
				packet >> ID;
				if (ID < clients.size())
				{
					for (int i = 0; i < clients.size(); i++)
					{
						if (clients[i].ID == ID && clients[i].address == client.address)
						{
							clients[i].ping = runtime - rTime;
						}
					}
				}
				packet.clear();
			}
				break;
			default:
				std::cout << "error: unexpected packet type" << std::endl;
				packet.clear();
				break;
			}
			m++;
		}

		while ((std::chrono::high_resolution_clock::now() - start) >
			std::chrono::milliseconds(runtime + dt))
		{
			runtime += dt;
			world.Update(dt);

			// Network send
			for (int i = 0; i < clients.size(); i++)
			{
				if (clients[i].dead)
					continue;
				packet.clear();
				MakeWorldPacket(packet, world, clients[i].ping, runtime);
				if (socket.send(packet, clients[i].address, clients[i].port) != sf::Socket::Done)
				{
					world.players[i].leaveTime = runtime;
					clients[i].dead = true;
					std::cout << "error: socket.send" << std::endl;
				}
				packet.clear();
			}
		}

		//window.clear();

		//window.display();
	}

	return 0;
}
