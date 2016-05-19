#include <RRDTP.h>
#include <iostream>

void ValueChanged(rrdtp::Connection* connection, rrdtp::Entry* entry)
{
	std::cout << "Recieved: \"" << entry->GetIdentifier() << "\ = ";
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
}

int main()
{
	rrdtp::Connection rrdtpConnection;

	std::cout << "Run as server? (y/n): ";
	char answer = 'y';
	std::cin >> answer;
	answer = tolower(answer);

	if (answer == 'y')
	{
		rrdtpConnection.StartServer();
	}
	else
	{
		std::string ip = "";
		std::cin >> ip;
		rrdtpConnection.StartClient(ip.c_str());
	}

	rrdtpConnection.SetValueChangedCallback(&ValueChanged);
	
	bool exitFlag = false;
	while (!exitFlag)
	{
		std::cout << "\nSelect action.\n";
		std::cout << "(a) send data\n";
		std::cout << "(b) receive data\n";
		std::cout << "(c) exit\n";

		char answer = 'y';
		std::cin >> answer;
		answer = tolower(answer);

		if (answer == 'a')
		{
			std::cout << "Please enter identifier: ";

			std::string ident = "";
			std::cin >> ident;

			std::cout << "Please select data type.\n";
			std::cout << "(a) int\n";
			std::cout << "(b) long\n";
			std::cout << "(c) float\n";
			std::cout << "(d) double\n";
			std::cout << "(e) boolean\n";
			std::cout << "(f) string\n";

			char dataType = 'a';
			std::cin >> dataType;
			dataType = tolower(dataType);

			if (dataType == 'a')
			{
				std::cout << "Please enter integer value: ";
				int val = 0;
				std::cin >> val;

				rrdtpConnection.SetInt(ident.c_str(), val);
			}
			else if (dataType == 'b')
			{
				std::cout << "Please enter long integer value: ";
				long val = 0;
				std::cin >> val;

				rrdtpConnection.SetLong(ident.c_str(), val);
			}
			else if (dataType == 'c')
			{
				std::cout << "Please enter float value: ";
				float val = 0;
				std::cin >> val;

				rrdtpConnection.SetFloat(ident.c_str(), val);
			}
			else if (dataType == 'd')
			{
				std::cout << "Please enter double value: ";
				double val = 0;
				std::cin >> val;

				rrdtpConnection.SetDouble(ident.c_str(), val);
			}
			else if (dataType == 'e')
			{
				std::cout << "Please enter boolean value: ";
				bool val = 0;
				std::cin >> val;

				rrdtpConnection.SetBool(ident.c_str(), val);
			}
			else if (dataType == 'f')
			{
				std::cout << "Please enter string value: ";
				std::string val = 0;
				std::cin >> val;

				rrdtpConnection.SetString(ident.c_str(), val.c_str());
			}
		}
		else if (answer == 'b')
		{
			std::cout << "\nReceiving data...\n";
		}
		else if (answer == 'c')
		{
			exitFlag = true;
		}

		rrdtpConnection.Poll();
	}

	return 0;
}