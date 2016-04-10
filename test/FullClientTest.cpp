#include <RRDTP.h>
#include <iostream>

int main()
{
	rrdtp::Connection client;
	client.StartClient("127.0.0.1");

	std::cout << "RRDTP Client test.\n\n";

	int oldVal = 0;
	while (true)
	{
		int newVal = client.GetInt("test-val");
		if (newVal != oldVal)
		{
			std::cout << "Recieved " << newVal << "\n";
			oldVal = newVal;
		}
	}
}