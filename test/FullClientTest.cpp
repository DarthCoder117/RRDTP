#include <RRDTP.h>
#include <RRDTP/Entry.h>
#include <iostream>
#include <cassert>

void ValueChanged(rrdtp::Connection* connection, rrdtp::Entry* entry)
{
	assert(strcmp(entry->GetName(), "test-val") == 0);

	std::cout << "Recieved " << ((rrdtp::IntEntry*)entry)->Get() << "\n";
}

int main()
{
	std::cout << "RRDTP Client test.\n\n";

	rrdtp::Connection client;
	client.StartClient("127.0.0.1");

	client.SetValueChangedCallback(&ValueChanged);

	int oldVal = 0;
	while (true)
	{
		client.Poll();

		int newVal = client.GetInt("test-val");
		if (newVal != oldVal)
		{
			std::cout << "GetInt returned " << newVal << "\n";
			oldVal = newVal;
		}
	}
}