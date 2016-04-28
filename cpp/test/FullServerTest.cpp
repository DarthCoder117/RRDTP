#include <RRDTP.h>
#include <iostream>

int main()
{
	rrdtp::Connection server;
	server.StartServer();

	std::cout << "RRDTP Server test.\n";
	std::cout << "Press enter when you want to send a new value.\n\n";

	for (int i = 1; i != 0; ++i)
	{
		std::cin.ignore();
		server.SetInt("test-val", i);
		std::cout << "Sending " << i << "\n";

		server.Poll();
	}

	return 0;
}