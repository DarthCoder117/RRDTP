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
#ifndef CInterface_H
#define CInterface_H
#ifdef  __cplusplus
#include "RRDTP/Types.h"
#include "RRDTP/Connection.h"
#include "RRDTP/Entry.h"
#include "RRDTP/Platform.h"
#endif

#if defined(RRDTP_PLATFORM_WINDOWS)
//  Microsoft 
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#elif defined(RRDTP_PLATFORM_UNIX)
//  GCC
#define DLL_EXPORT __attribute__((visibility("default")))
#define DLL_IMPORT
#endif

typedef void rrdtp_Connection;
typedef void rrdtp_Category;
typedef void rrdtp_Entry;
typedef void (*rrdtp_ValueChangedCallback)(rrdtp_Connection* connection, rrdtp_Entry* entry);

extern "C"
{
	//Connection
	DLL_EXPORT rrdtp_Connection* rrdtp_OpenServerConnection(unsigned int port);
	DLL_EXPORT rrdtp_Connection* rrdtp_OpenClientConnection(const char* ip, unsigned int port);

	DLL_EXPORT void rrdtp_CloseConnection(rrdtp_Connection* connection);

	DLL_EXPORT void rrdtp_PollConnection(rrdtp_Connection* connection);

	DLL_EXPORT void rrdtp_SetInt(rrdtp_Connection* connection, const char* identifier, int val);
	DLL_EXPORT int rrdtp_GetInt(rrdtp_Connection* connection, const char* identifier, int defaultVal);

	DLL_EXPORT void rrdtp_SetLong(rrdtp_Connection* connection, const char* identifier, int64_t val);
	DLL_EXPORT int64_t rrdtp_GetLong(rrdtp_Connection* connection, const char* identifier, int64_t defaultVal);

	DLL_EXPORT void rrdtp_SetFloat(rrdtp_Connection* connection, const char* identifier, float val);
	DLL_EXPORT float rrdtp_GetFloat(rrdtp_Connection* connection, const char* identifier, float defaultVal);

	DLL_EXPORT void rrdtp_SetDouble(rrdtp_Connection* connection, const char* identifier, double val);
	DLL_EXPORT double rrdtp_GetDouble(rrdtp_Connection* connection, const char* identifier, double defaultVal);

	DLL_EXPORT void rrdtp_SetBool(rrdtp_Connection* connection, const char* identifier, bool val);
	DLL_EXPORT bool rrdtp_GetBool(rrdtp_Connection* connection, const char* identifier, bool defaultVal);

	DLL_EXPORT void rrdtp_SetString(rrdtp_Connection* connection, const char* identifier, const char* str);
	DLL_EXPORT const char* rrdtp_GetString(rrdtp_Connection* connection, const char* identifier, const char* defaultVal);
	
	DLL_EXPORT rrdtp_Category* rrdtp_GetCategory(rrdtp_Connection* connection, const char* identifier);

	DLL_EXPORT rrdtp_Entry* rrdtp_GetEntry(rrdtp_Connection* connection, const char* identifier);

	DLL_EXPORT void rrdtp_DeleteEntry(rrdtp_Connection* connection, const char* identifier);

	DLL_EXPORT void rrdtp_SetValueChangedCallback(rrdtp_Connection* connection, rrdtp_ValueChangedCallback callback);

	//Entry
	DLL_EXPORT const char* rrdtp_Entry_GetName(rrdtp_Entry* entry);

	DLL_EXPORT const char* rrdtp_Entry_GetIdentifier(rrdtp_Entry* entry);

	DLL_EXPORT int rrdtp_Entry_GetType(rrdtp_Entry* entry);

	DLL_EXPORT void rrdtp_IntEntry_Set(rrdtp_Entry* entry, int val);
	DLL_EXPORT int rrdtp_IntEntry_Get(rrdtp_Entry* entry);

	DLL_EXPORT void rrdtp_LongEntry_Set(rrdtp_Entry* entry, int64_t val);
	DLL_EXPORT int64_t rrdtp_LongEntry_Get(rrdtp_Entry* entry);

	DLL_EXPORT void rrdtp_FloatEntry_Set(rrdtp_Entry* entry, float val);
	DLL_EXPORT float rrdtp_FloatEntry_Get(rrdtp_Entry* entry);

	DLL_EXPORT void rrdtp_DoubleEntry_Set(rrdtp_Entry* entry, double val);
	DLL_EXPORT double rrdtp_DoubleEntry_Get(rrdtp_Entry* entry);

	DLL_EXPORT void rrdtp_BooleanEntry_Set(rrdtp_Entry* entry, bool val);
	DLL_EXPORT bool rrdtp_BooleanEntry_Get(rrdtp_Entry* entry);

	DLL_EXPORT void rrdtp_StringEntry_Set(rrdtp_Entry* entry, const char* str);
	DLL_EXPORT const char* rrdtp_StringEntry_Get(rrdtp_Entry* entry);
}

#endif
