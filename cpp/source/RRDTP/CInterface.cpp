/*==============================================================================
The MIT License (MIT)

Copyright (c) 2016 Tanner Mickelson & The RRDTP Team

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

	void rrdtp_SetFloat(rrdtp_Connection* connection, const char* identifier, float val)
	{
		((rrdtp::Connection*)connection)->SetFloat(identifier, val);
	}
	float rrdtp_GetFloat(rrdtp_Connection* connection, const char* identifier, float defaultVal)
	{
		return ((rrdtp::Connection*)connection)->GetFloat(identifier, defaultVal);
	}

	void rrdtp_SetDouble(rrdtp_Connection* connection, const char* identifier, double val)
	{
		((rrdtp::Connection*)connection)->SetDouble(identifier, val);
	}
	double rrdtp_GetDouble(rrdtp_Connection* connection, const char* identifier, double defaultVal)
	{
		return ((rrdtp::Connection*)connection)->GetDouble(identifier, defaultVal);
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

	rrdtp_Category* rrdtp_GetCategory(rrdtp_Connection* connection, const char* identifier)
	{
		return (rrdtp_Category*)((rrdtp::Connection*)connection)->GetCategory(identifier);
	}

	rrdtp_Entry* rrdtp_GetEntry(rrdtp_Connection* connection, const char* identifier)
	{
		return (rrdtp_Entry*)((rrdtp::Connection*)connection)->GetEntry(identifier);
	}

	void rrdtp_DeleteEntry(rrdtp_Connection* connection, const char* identifier)
	{
		((rrdtp::Connection*)connection)->Delete(identifier);
	}

	void rrdtp_SetValueChangedCallback(rrdtp_Connection* connection, rrdtp_ValueChangedCallback callback)
	{
		((rrdtp::Connection*)connection)->SetValueChangedCallback((rrdtp::ValueChangedCallback)callback);
	}

	//Entry
	const char* rrdtp_Entry_GetName(rrdtp_Entry* entry)
	{
		return ((rrdtp::Entry*)entry)->GetName();
	}

	const char* rrdtp_Entry_GetIdentifier(rrdtp_Entry* entry)
	{
		return ((rrdtp::Entry*)entry)->GetIdentifier();
	}

	int rrdtp_Entry_GetType(rrdtp_Entry* entry)
	{
		return (int)((rrdtp::Entry*)entry)->GetType();
	}

	void rrdtp_IntEntry_Set(rrdtp_Entry* entry, int val)
	{
		((rrdtp::IntEntry*)entry)->Set(val);
	}
	int rrdtp_IntEntry_Get(rrdtp_Entry* entry)
	{
		return ((rrdtp::IntEntry*)entry)->Get();
	}

	void rrdtp_LongEntry_Set(rrdtp_Entry* entry, int64_t val)
	{
		((rrdtp::LongEntry*)entry)->Set(val);
	}
	int64_t rrdtp_LongEntry_Get(rrdtp_Entry* entry)
	{
		return ((rrdtp::LongEntry*)entry)->Get();
	}

	void rrdtp_FloatEntry_Set(rrdtp_Entry* entry, float val)
	{
		((rrdtp::FloatEntry*)entry)->Set(val);
	}
	float rrdtp_FloatEntry_Get(rrdtp_Entry* entry)
	{
		return ((rrdtp::FloatEntry*)entry)->Get();
	}

	void rrdtp_DoubleEntry_Set(rrdtp_Entry* entry, double val)
	{
		((rrdtp::DoubleEntry*)entry)->Set(val);
	}
	double rrdtp_DoubleEntry_Get(rrdtp_Entry* entry)
	{
		return ((rrdtp::DoubleEntry*)entry)->Get();
	}

	void rrdtp_BooleanEntry_Set(rrdtp_Entry* entry, bool val)
	{
		((rrdtp::BooleanEntry*)entry)->Set(val);
	}
	bool rrdtp_BooleanEntry_Get(rrdtp_Entry* entry)
	{
		return ((rrdtp::BooleanEntry*)entry)->Get();
	}

	void rrdtp_StringEntry_Set(rrdtp_Entry* entry, const char* str)
	{
		((rrdtp::StringEntry*)entry)->Set(str);
	}
	const char* rrdtp_StringEntry_Get(rrdtp_Entry* entry)
	{
		return ((rrdtp::StringEntry*)entry)->Get();
	}
}