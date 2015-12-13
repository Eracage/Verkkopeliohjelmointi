#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#include <thread>
#include <mutex>

#include <iostream>
#include <sstream>

#include <vector>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 50000

std::mutex printMutex;

void ThreadSafePrint(std::string string)
{
	printMutex.lock();
	std::cout << string << std::endl;
	printMutex.unlock();
}

void ClientListener(SOCKET ClientSocket, int ThreadNum)
{
	int iResult;
	int iSendResult;

	std::stringstream text;

	char buffer[DEFAULT_BUFLEN];

	text << "Customer " << ThreadNum << " Connected: " << std::endl;
	ThreadSafePrint(text.str());
	text.str(std::string());

	std::string welcomeText = "Welcome to Restaurant OY. Enter your order please:";
	strncpy_s(buffer, welcomeText.c_str(), welcomeText.length() + 1);

	iResult = send(ClientSocket, buffer, (int)strlen(buffer), 0);
	if (iResult == SOCKET_ERROR)
	{
		text << "send failed with error: " << WSAGetLastError() << std::endl;
		ThreadSafePrint(text.str());
		closesocket(ClientSocket);
		return;
	}

	do {
		// TODO: Write Communication Logic

		iResult = recv(ClientSocket, buffer, DEFAULT_BUFLEN, 0);
		if (iResult > 0)
		{
			text << "Customer " << ThreadNum << " Ordered: ";
			text.write(buffer, iResult);
			ThreadSafePrint(text.str());
			text.str(std::string());

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, buffer, iResult, 0);
			if (iSendResult == SOCKET_ERROR)
			{
				text << "Thread " << ThreadNum << "send failed with error: " << WSAGetLastError() << std::endl;
				ThreadSafePrint(text.str());

				closesocket(ClientSocket);
				return;
			}
		}
		else if (iResult == 0)
		{
			text << "Customer " << ThreadNum << " Disconnecting..." << std::endl;
			ThreadSafePrint(text.str());
			text.str(std::string());
		}
		else
		{
			text << "Thread " << ThreadNum << "recv failed with error: " << WSAGetLastError() << std::endl;
			ThreadSafePrint(text.str());
			text.str(std::string());

			closesocket(ClientSocket);
			return;
		}

	} while (iResult > 0);

	closesocket(ClientSocket);
}

int main()
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	SOCKET NewListenSocket = INVALID_SOCKET;

	struct addrinfo *result = nullptr;
	struct addrinfo hints;

	int iSendResult;
	char buffer[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	std::cout << "Initializing Restaurant Server..." << std::endl;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << "WSAStartup failed with error: " << iResult << std::endl;
		return 1;
	}

	std::vector<std::thread*> ClientListeners;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;


	u_short nextSocket = DEFAULT_PORT;


	// Resolve the server address and port
	iResult = getaddrinfo(nullptr, std::to_string(DEFAULT_PORT).c_str(), &hints, &result);
	if (iResult != 0)
	{
		std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "listen failed with error: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Initialization Complete, waiting for Customers:" << std::endl << std::endl;

	// Accept new connections and create sockets and threads for them
	while (true)
	{
		// Accept a client socket
		ClientSocket = accept(ListenSocket, nullptr, nullptr);
		if (ClientSocket == INVALID_SOCKET)
		{
			std::cout << "accept failed with error: " << WSAGetLastError() << std::endl;
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		// Create new thread for new connection
		ClientListeners.push_back(new std::thread(ClientListener, ClientSocket, ClientListeners.size()));

		//closesocket(ClientSocket);
		ClientSocket = INVALID_SOCKET;
	}
	// while true so it never gets here... but let's keep them here if we develop this further some time

	for each (std::thread* thread in ClientListeners)
	{
		thread->join();
	}

	// No longer need listener socket
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}
