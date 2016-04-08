#include "RRDTP.h"
#include "RRDTP/Sockets/WinsockSocket.h"
#include <iostream>
#include <cassert>

void serverDataRecieved(rrdtp::Socket* socket, rrdtp::HostID from, void* data, size_t dataSz)
{
	long val = *(long*)data;
	std::cout << "Server recieved value: " << val << "\n";
}

void clientDataRecieved(rrdtp::Socket* socket, rrdtp::HostID from, void* data, size_t dataSz)
{
	long val = *(long*)data;
	std::cout << "Client recieved value: " << val << "\n";
}

int main()
{
	std::cout << "=========================================================================\n";
	std::cout << "TCP socket test\n";
	std::cout << "=========================================================================\n";

	std::cout << "Creating server socket.\n";

	//Create server and start listening for connections.
	rrdtp::Socket* serverSocket = rrdtp::Socket::Create(&serverDataRecieved);
	if (serverSocket->Listen(4309, rrdtp::ESP_TCP) == rrdtp::ESE_SUCCESS)
	{
		std::cout << "Server listening on port 4309.\n";
	}
	else
	{
		std::cout << "Error binding socket.\n";
		return -1;
	}
	
	std::cout << "Creating client socket.\n";

	//Create client socket and try to connect to the server.
	rrdtp::Socket* clientSocket = rrdtp::Socket::Create(&clientDataRecieved);
	if (clientSocket->Connect("127.0.0.1", 4309, rrdtp::ESP_TCP) == rrdtp::ESE_SUCCESS)
	{
		std::cout << "Client connected.\n";
	}
	else
	{
		std::cout << "Error connecting to server.\n";
		return -1;
	}

	//Accept connection on server
	rrdtp::HostID clientID = serverSocket->Accept();

	//Send data to server
	long val = 12345678;//Not bothering to convert to network order here, because this is just a local test.
	std::cout << "Server sending value: " << val << "\n";
	size_t actuallySent = clientSocket->Send(clientID, &val, sizeof(val));
	assert(actuallySent == sizeof(val));

	//Poll for recieved messages on the server.
	//This will usually be done in a loop, but this socket test program is only for testing the socket code.
	serverSocket->Poll();

	system("pause");
}