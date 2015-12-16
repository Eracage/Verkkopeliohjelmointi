#include <Client.h>
#include <iostream>
#include <thread>

Client::Client(World & w)
	: world(w)
{
	socket.setBlocking(false);
	ownData.disconnected = true;
	sWorldRunTime = 0;
}

Client::~Client()
{
	packet.clear();
	MakeDCPacket(packet);
	status = socket.send(packet, server.address, server.port);
}

bool Client::Connect(std::string ip, std::string port, std::string ownPort, std::string ownName)
{
	server.address = ip;
	server.port = std::stoi(port);

	ownData.name = ownName;

	if (ownPort.length() > 0)
	{
		ownData.port = std::stoi(ownPort);
		status = socket.bind(ownData.port);
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

	packet.clear();
	MakeConnectPacket(packet, ownData.name);
	status = socket.send(packet, server.address, server.port);
	switch (status)
	{
	case sf::Socket::Done:
		break;
	default:
		std::cout << "error: socket.send" << std::endl;
		return false;
	}

	for (int i = 0; i < 100; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		packet.clear();
		status = socket.receive(packet, remote.address, remote.port);
		switch (status)
		{
		case sf::Socket::Done:
			packet >> pType;
			switch (pType)
			{
			case 17:
				packet >> ServerTimeDif;
				packet >> ownData.ID;
				if (server.port == remote.port)
					server.address = remote.address;

				ownData.disconnected = false;
				return true;
			default:
				std::cout << "error: unexpected packet type at connect " << pType << std::endl;
				return false;
			}
			return false;
		case sf::Socket::NotReady:
			break;
		default:
			return false;
		}
	}

	return false;
}

bool Client::Receive(sf::Int32 runtime)
{
	packet.clear();
	status = socket.receive(packet, remote.address, remote.port);
	switch (status)
	{
	case sf::Socket::Done:

		if (remote.address != server.address || remote.port != server.port)
		{
			std::cout << "error: unrecognized remote" << std::endl;
			return true;
		}

		packet >> pType;

		switch (pType)
		{
		case 15:
			{
				sf::Int32 serverRunTime;
				sf::Uint32 bSize;
				sf::Uint32 pSize;

				packet >> serverRunTime;

				if (sWorldRunTime > serverRunTime)
					break;

				sWorldRunTime = serverRunTime;

				world.runTime = serverRunTime;
				ServerTimeDif = serverRunTime - runtime;

				packet >> server.ping;
				ownData.ping = server.ping;

				packet >> bSize;
				for (size_t i = 0; i < bSize; i++)
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
					}
				}

				packet >> pSize;
				for (size_t i = 0; i < pSize; i++)
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
					}
				}

				packet.clear();
				MakeReceivedPacket(packet,ownData.ID,serverRunTime);
				status = socket.send(packet,server.address,server.port);
				switch (status)
				{
				case sf::Socket::Done:
					return true;
				default:
					std::cout << "error: received packet send failed" << std::endl;
					return true;
				}
			}
			return true;
		case 16:
			ownData.disconnected = true;
		case 17:
			std::cout << "extra received package" << std::endl;
			return true;
		default:
			std::cout << "error: unexpected packet type" << std::endl;
			break;
		}


		return true;
	case sf::Socket::NotReady:
		return false;
	case sf::Socket::Disconnected:
		std::cout << "error: something disconnected?" << std::endl;
		return false;
	case sf::Socket::Error:
		std::cout << "error: receive" << std::endl;
		return false;
	default:
		return false;
	}

	return false;
}

void Client::Update()
{
	if (world.players.size() <= ownData.ID)
	{
		std::cout << "not ready yet" << std::endl;
		return;
	}
	packet.clear();
	MakePlayerUpdatePacket(packet, world.players[ownData.ID]);
	status = socket.send(packet, server.address, server.port);
	switch (status)
	{
	case sf::Socket::Done:
		break;
	default:
		std::cout << "error: playerUpdate" << std::endl;
		break;
	}
}
