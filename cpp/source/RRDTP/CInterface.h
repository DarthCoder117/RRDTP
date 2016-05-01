/*==============================================================================
The MIT License (MIT)

Copyright (c) 2016 Tanner Mickelson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
==============================================================================*/
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

	void rrdtp_PollConnection(rrdtp_
	Connection* connection);

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