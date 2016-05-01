#ifndef CInterface_H
#define CInterface_H
#ifdef  __cplusplus
#include "RRDTP/Types.h"
#include "RRDTP/Connection.h"
#include "RRDTP/Entry.h"
#endif

typedef void rrdtp_Connection;
typedef void rrdtp_Entry;

extern "C"
{
	//Connection
	rrdtp_Connection* rrdtp_OpenServerConnection(unsigned int port);
	rrdtp_Connection* rrdtp_OpenClientConnection(const char* ip, unsigned int port);

	void rrdtp_CloseConnection(rrdtp_Connection* connection);

	void rrdtp_PollConnection(rrdtp_Connection* connection);

	//Entries
	void rrdtp_SetInt(rrdtp_Connection* connection, const char* identifier, int val);
	int rrdtp_GetInt(rrdtp_Connection* connection, const char* identifier, int defaultVal);

	void rrdtp_SetLong(rrdtp_Connection* connection, const char* identifier, int64_t val);
	int64_t rrdtp_GetLong(rrdtp_Connection* connection, const char* identifier, int64_t defaultVal);

	void rrdtp_SetBool(rrdtp_Connection* connection, const char* identifier, bool val);
	bool rrdtp_GetBool(rrdtp_Connection* connection, const char* identifier, bool defaultVal);

	void rrdtp_SetString(rrdtp_Connection* connection, const char* identifier, const char* str);
	const char* rrdtp_GetString(rrdtp_Connection* connection, const char* identifier, const char* defaultVal);
}

#endif