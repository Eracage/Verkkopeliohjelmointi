#define WIN32_LEAN_AND_MEAN

#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <World.h>
#include <iostream>
#include <regex>

#include <Server.h>

#include <Regexer.h>
#include <NetworkFunctions.h>

int main(int argc, char* argv[])
{
	for (size_t i = 0; i < argc; i++)
	{
		std::cout << argv[i] << std::endl;
	}
	std::cout << std::endl;

	World world;

	Server server(world);

	std::string portString = "27012";

	if (argc > 1)
		portString = argv[1];

	if (!server.Bind(portString))
		if (server.Bind(""))
			return -1;

	std::cout << "Socket bind at port " << server.port << std::endl;

	auto start = std::chrono::high_resolution_clock::now();

	sf::Int32 dt = 10;
	sf::Int32 runtime = 0;

	while (1)
	{
		for (size_t i = 0; server.Receive(runtime) && i < 10000; i++);

		if ((std::chrono::high_resolution_clock::now() - start) >
			std::chrono::milliseconds(runtime + dt))
		{
			runtime += dt;

			world.Update(dt);

			server.Update(runtime);
		}
	}

	return 0;
}
