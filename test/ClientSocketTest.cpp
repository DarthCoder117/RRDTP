#include <RRDTP.h>
#include <iostream>
#include <cassert>

void dataRecieved(rrdtp::Socket* self, rrdtp::HostID sender, void* data, size_t sz)
{
	long val = *(long*)data;
	std::cout << "Client recieved value: " << val << "\n";
}

int main()
{
	std::cout << "=========================================================================\n";
	std::cout << "TCP client socket test\n";
	std::cout << "=========================================================================\n";

	std::cout << "Creating client socket.\n";

	//Create client socket and try to connect to the server.
	rrdtp::Socket* clientSocket = rrdtp::Socket::Create(&dataRecieved);
	if (clientSocket->Connect("127.0.0.1", 4309, rrdtp::ESP_TCP) == rrdtp::ESE_SUCCESS)
	{
		std::cout << "Client connected.\n";
	}
	else
	{
		std::cout << "Error connecting to server.\n";
		return -1;
	}

	//Send data to server
	long val = 12345678;//Not bothering to convert to network order here, because this is just a local test.
	std::cout << "Client sending value: " << val << "\n";
	size_t actuallySent = clientSocket->Send(&val, sizeof(val));
	assert(actuallySent == sizeof(val));

	//Poll for recieved messages on the server.
	//This will usually be done in a loop, but this socket test program is only for testing the socket code.
	while (true)
	{
		clientSocket->Poll();
	}

	system("pause");
}