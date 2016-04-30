#ifndef CInterface_H
#define CInterface_H
#ifdef  __cplusplus
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

}

#endif