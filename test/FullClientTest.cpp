#include <RRDTP.h>
#include <iostream>
#include <cassert>

void ValueSet(rrdtp::Connection* connection, rrdtp::Entry* entry)
{
	assert(strcmp(entry->GetName(), "test-val") == 0);

	std::cout << "Recieved " << entry->Get<int>() << "\n";
}

int main()
{
	std::cout << "RRDTP Client test.\n\n";

	rrdtp::Connection client;
	client.StartClient("127.0.0.1");

	client.SetValueSetCallback(&ValueSet);

	int oldVal = 0;
	while (true)
	{
		client.Poll();

		/*int newVal = client.GetInt("test-val");
		if (newVal != oldVal)
		{
			std::cout << "Recieved " << newVal << "\n";
			oldVal = newVal;
		}*/
	}
}