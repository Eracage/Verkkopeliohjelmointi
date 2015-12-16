#include <Server.h>
#include <iostream>

Server::Server(World & w)
	: world(w)
{
	socket.setBlocking(false);
	disconnectedClients = connectedClients = 0;
}

Server::~Server()
{
}

bool Server::Bind(std::string p)
{
	if (p.length() > 0)
	{
		port = std::stoi(p);
		status = socket.bind(port);
		switch (status)
		{
		case sf::Socket::Done:
			break;
		default:
			std::cout << "error: socket.bind" << std::endl;
			return false;
		}
	}
	else
	{
		status = socket.bind(sf::Socket::AnyPort);
		switch (status)
		{
		case sf::Socket::Done:
			break;
		default:
			std::cout << "error: socket.bind anyport" << std::endl;
			return false;
		}
	}

	return true;
}

bool Server::Receive(sf::Int32 runtime)
{
	packet.clear();
	status = socket.receive(packet, remote.address, remote.port);
	switch (status)
	{
	case sf::Socket::Done:

		packet >> pType;

		switch (pType)
		{
		case 13:
			{
				packet >> remote.name;
				remote.ID = clients.size();
				std::cout << remote.name << " ID: " << remote.ID << " Connecting... " << remote.address << ":" << remote.port << std::endl;

				for (size_t i = 0; i < clients.size(); i++)
				{
					if (remote.address == clients[i].address &&
						remote.port == clients[i].port)
						remote.ID = i;
				}
				if (remote.ID == clients.size())
				{
					remote.ping = 0;
					clients.push_back(ConnectionData(remote));
					world.AddPlayer(runtime);
					connectedClients++;
				}
				clients[remote.ID].lastUpdate = runtime;
				clients[remote.ID].maybeDisconnected = false;
				clients[remote.ID].disconnected = false;

				packet.clear();
				MakeReceivedPacket(packet, remote.ID, runtime);
				status = socket.send(packet, remote.address, remote.port);
				switch (status)
				{
				case sf::Socket::Done:
					return true;
				default:
					std::cout << "error: sending receive package failed" << std::endl;
					return true;
				}
			}
			return false;
		case 14:
			{
				WorldPlayer player;
				packet >> player;

				if (player.ID >= clients.size())
				{
					std::cout << "error: player.ID >= clients.size()" << std::endl;
					return true;
				}
				if (clients[player.ID].address != remote.address)
				{
					std::cout << "error: clients[player.ID].address != client.address" << std::endl;
					return true;
				}
				clients[player.ID].lastUpdate = runtime;
				clients[player.ID].maybeDisconnected = false;
				//clients[player.ID].disconnected = false;

				world.UpdatePlayer(player);
			}
			return true;
		case 16:
			{
				sf::Uint32 ID = -1;
				for (size_t i = 0; i < clients.size(); i++)
				{
					if (clients[i].address == remote.address && clients[i].port == remote.port)
					{
						ID = i;
					}
				}
				world.players[ID].leaveTime = runtime;
				clients[ID].disconnected = true;
				disconnectedClients++;
				std::cout << "disconnected via message " << ID << std::endl;
			}
		case 17:
			{
				sf::Uint32 ID;
				sf::Int32 rTime;

				packet >> rTime;
				packet >> ID;
				if (ID < clients.size())
				{
					for (size_t i = 0; i < clients.size(); i++)
					{
						if (clients[i].ID == ID && clients[i].address == remote.address)
						{
							clients[i].ping = runtime - rTime;
							clients[i].lastUpdate = runtime;
							clients[i].maybeDisconnected = false;
						}
					}
				}
			}
			return true;
		default:
			std::cout << "error: unexpected packet type" << std::endl;
			break;
		}


		return true;
	case sf::Socket::NotReady:
		return false;
	case sf::Socket::Disconnected:
		if (connectedClients + disconnectedClients == clients.size())
		{
			disconnectedClients++;

			MakeDirty();
		}
		//std::cout << "disconnected " << remote.address << ":" << remote.port << std::endl;
		return false;
	case sf::Socket::Error:
		std::cout << "error: receive" << std::endl;
		return false;
	default:
		return false;
	}

	return false;
}

void Server::Update(sf::Int32 runtime)
{
	int dcCount = 0;
	bool knownDC = false;

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].maybeDisconnected)
		{
			dcCount++;
		}
	}

	if (dcCount == 1 + clients.size() - connectedClients - disconnectedClients)
	{
		knownDC = true;
	}

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (knownDC && clients[i].maybeDisconnected && !clients[i].disconnected)
		{
			clients[i].disconnected = true;
			disconnectedClients++;
			world.players[i].leaveTime = runtime;
			std::cout << "disconnected via search " << i << std::endl;
		}

		if (clients[i].lastUpdate < runtime - 10000 && clients[i].maybeDisconnected && !clients[i].disconnected)
		{
			clients[i].disconnected = true;
			disconnectedClients++;
			world.players[i].leaveTime = runtime;
			std::cout << "disconnected via timeout " << i << std::endl;

			MakeDirty();
		}

		if (clients[i].disconnected)
		{
			continue;
		}

		packet.clear();
		MakeWorldPacket(packet, world, clients[i].ping, runtime);
		if (socket.send(packet, clients[i].address, clients[i].port) != sf::Socket::Done)
		{
			world.players[i].leaveTime = runtime;
			clients[i].disconnected = true;
			disconnectedClients++;
			std::cout << "error: disconnected " << i << " via socket send failure" << std::endl;

			MakeDirty();
		}
		packet.clear();
	}
}

void Server::MakeDirty()
{
	if (connectedClients + disconnectedClients != clients.size())
	{
		for (size_t i = 0; i < clients.size(); i++)
		{
			clients[i].maybeDisconnected = true;
		}
	}
}
