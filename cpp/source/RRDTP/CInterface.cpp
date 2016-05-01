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
	void rrdtp_SetInt(rrdtp_Connection* connection, const char* identifier, int val)
	{
		((rrdtp::Connection*)connection)->SetInt(identifier, val);
	}
	int rrdtp_GetInt(rrdtp_Connection* connection, const char* identifier, int defaultVal)
	{
		return ((rrdtp::Connection*)connection)->GetInt(identifier, defaultVal);
	}

	void rrdtp_SetLong(rrdtp_Connection* connection, const char* identifier, int64_t val)
	{
		((rrdtp::Connection*)connection)->SetLong(identifier, val);
	}
	int64_t rrdtp_GetLong(rrdtp_Connection* connection, const char* identifier, int64_t defaultVal)
	{
		return ((rrdtp::Connection*)connection)->GetLong(identifier, defaultVal);
	}

	void rrdtp_SetBool(rrdtp_Connection* connection, const char* identifier, bool val)
	{
		((rrdtp::Connection*)connection)->SetBool(identifier, val);
	}
	bool rrdtp_GetBool(rrdtp_Connection* connection, const char* identifier, bool defaultVal)
	{
		return ((rrdtp::Connection*)connection)->GetBool(identifier, defaultVal);
	}

	void rrdtp_SetString(rrdtp_Connection* connection, const char* identifier, const char* str)
	{
		((rrdtp::Connection*)connection)->SetString(identifier, str);
	}
	const char* rrdtp_GetString(rrdtp_Connection* connection, const char* identifier, const char* defaultVal)
	{
		return ((rrdtp::Connection*)connection)->GetString(identifier, defaultVal);
	}
}