#include <RRDTP.h>
#include <iostream>
#include <ctime>

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
		std::cout << "Please enter IP: ";
		std::string ip = "";
		std::cin >> ip;
		rrdtpConnection.StartClient(ip.c_str());
	}

	//Test float
	float testFloat = 0.0f;
	bool increaseFlag = true;

	bool testBool = true;

	srand(time(NULL));

	bool exitFlag = false;
	while (!exitFlag)
	{
		if (increaseFlag)
		{
			testFloat += 1.0f;

			if (testFloat >= 150.0f)
			{
				increaseFlag = false;
				testFloat = 150.0f;
			}
		}
		else
		{
			testFloat -= 0.5f;

			if (testFloat <= 0.0f)
			{
				increaseFlag = true;
				testFloat = 0.0f;
			}
		}

		rrdtpConnection.SetFloat("sensors.pulse", testFloat);

		rrdtpConnection.SetInt("sensors.random", rand()%500);

		rrdtpConnection.SetBool("flags.increasing", increaseFlag);

		rrdtpConnection.Poll();
		
		Sleep(50);
	}

	return 0;
}