#include <RRDTP.h>
#include <iostream>
#include <cassert>

void dataRecieved(rrdtp::Socket* self, rrdtp::HostID sender, void* data, size_t dataSz)
{
	long val = *(long*)data;
	std::cout << "Server recieved value: " << val << "\n";

	val = 54321;
	std::cout << "Server sending value back: " << val << "\n";
	self->Send(&val, sizeof(val), sender);
}

int main()
{
	std::cout << "=========================================================================\n";
	std::cout << "TCP socket server test\n";
	std::cout << "=========================================================================\n";

	std::cout << "Creating server socket.\n";

	//Create server and start listening for connections.
	rrdtp::Socket* serverSocket = rrdtp::Socket::Create(&dataRecieved);
	if (serverSocket->Listen(4309, rrdtp::ESP_TCP) == rrdtp::ESE_SUCCESS)
	{
		std::cout << "Server listening on port 4309.\n";
	}
	else
	{
		std::cout << "Error binding socket.\n";
		return -1;
	}
	
	while (true)
	{
		//Accept connections on server
		if (serverSocket->Accept() != -1)
		{
			std::cout << "Server accepted connection.\n";
		}

		serverSocket->Poll();
	}

	system("pause");
}