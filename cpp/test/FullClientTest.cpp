#include <RRDTP.h>
#include <RRDTP/Entry.h>
#include <iostream>
#include <cassert>

void ValueChanged(rrdtp::Connection* connection, rrdtp::Entry* entry)
{
	std::cout << "Recieved: \"" << entry->GetIdentifier() << "\" = ";
	if (entry->GetType() == rrdtp::EDT_INT)
	{
		rrdtp::IntEntry* newEnt = (rrdtp::IntEntry*)entry;
		std::cout << newEnt->Get();
	}
	else if (entry->GetType() == rrdtp::EDT_LONG)
	{
		rrdtp::LongEntry* newEnt = (rrdtp::LongEntry*)entry;
		std::cout << newEnt->Get();
	}
	else if (entry->GetType() == rrdtp::EDT_FLOAT)
	{
		rrdtp::FloatEntry* newEnt = (rrdtp::FloatEntry*)entry;
		std::cout << newEnt->Get();
	}
	else if (entry->GetType() == rrdtp::EDT_DOUBLE)
	{
		rrdtp::DoubleEntry* newEnt = (rrdtp::DoubleEntry*)entry;
		std::cout << newEnt->Get();
	}
	else if (entry->GetType() == rrdtp::EDT_BOOLEAN)
	{
		rrdtp::BooleanEntry* newEnt = (rrdtp::BooleanEntry*)entry;
		std::cout << newEnt->Get();
	}
	else if (entry->GetType() == rrdtp::EDT_STRING)
	{
		rrdtp::StringEntry* newEnt = (rrdtp::StringEntry*)entry;
		std::cout << "\"" << newEnt->Get() << "\"";
	}

	std::cout << "\n";
	//std::cout << "Recieved " << ((rrdtp::IntEntry*)entry)->Get() << "\n";
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

		int newVal = client.GetBool("flags.i");
		if (newVal != oldVal)
		{
			std::cout << "GetInt returned " << newVal << "\n";
			oldVal = newVal;
		}
	}
}