#include "CInterface.h"

extern "C"
{
	//Connection
	rrdtp_Connection* rrdtp_OpenServerConnection(unsigned int port)
	{
		rrdtp::Connection* connection = new rrdtp::Connection();
		connection->StartServer(port);
		return (rrdtp_Connection*)connection;
	}

	rrdtp_Connection* rrdtp_OpenClientConnection(const char* ip, unsigned int port)
	{
		rrdtp::Connection* connection = new rrdtp::Connection();
		connection->StartClient(ip, port);
		return (rrdtp_Connection*)connection;
	}

	void rrdtp_CloseConnection(rrdtp_Connection* connection)
	{
		delete (rrdtp::Connection*)connection;
	}

	void rrdtp_PollConnection(rrdtp_Connection* connection)
	{
		((rrdtp::Connection*)connection)->Poll();
	}

	//Entries
}