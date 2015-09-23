/*
Simple udp client
Silver Moon (m00n.silv3r@gmail.com)
*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<winsock2.h>

#include <thread>
#include <mutex>
#include <chrono>

#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

void ListenServer(SOCKET s, sockaddr_in* si_other)
{
	int slen = sizeof(si_other);
	while (true)
	{
		//std::this_thread::sleep_for(std::chrono::seconds(50));
		char buf[BUFLEN] = { 0 };
		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		////memset(buf, '\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		puts(buf);
	}
}

int main(void)
{
	struct sockaddr_in client, si_other;
	int s, slen = sizeof(si_other);
	//char buf[BUFLEN];
	char message[BUFLEN];
	WSADATA wsa;


	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);


	int port = PORT + 1;

	memset((char *)&client, 0, sizeof(client));

	do {
		if (port != PORT + 1)
		{
			printf("socket() failed with error code : %d", WSAGetLastError());
		}
		printf("Trying port %d\n", port);

		memset((char *)&client, 0, sizeof(client));
		client.sin_family = AF_INET;
		client.sin_addr.S_un.S_addr = INADDR_ANY;
		client.sin_port = htons(port++);

	} while ((bind(s, (struct sockaddr *)&client, sizeof(client)) == SOCKET_ERROR && port < 65500));

	auto thread = std::thread(ListenServer, s, &si_other);

	//start communication
	while (1)
	{
		//std::this_thread::sleep_for(std::chrono::microseconds(50000));
		printf("Enter message : ");
		gets(message);

		//send the message
		if (sendto(s, message, strlen(message), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		////receive a reply and print it
		////clear the buffer by filling null, it might have previously received data
		//memset(buf, '\0', BUFLEN);
		////try to receive some data, this is a blocking call
		//if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
		//{
		//	printf("recvfrom() failed with error code : %d", WSAGetLastError());
		//	exit(EXIT_FAILURE);
		//}

		//puts(buf);
	}

	thread.join();

	closesocket(s);
	WSACleanup();

	return 0;
}
