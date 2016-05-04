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
#include "RRDTP/Entry.h"
#include "RRDTP/Platform.h"
#include <cassert>
#ifdef RRDTP_PLATFORM_WINDOWS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock2.h>
#include <windows.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#endif

using namespace rrdtp;

Entry::Entry(HostID owner, const char* identifier)
	:m_owner(owner),
	m_identifier(identifier),
	m_name(NULL)
{
	assert(identifier != NULL);

	//Set name pointer based on last index of '.'
	m_name = strrchr(identifier, '.');
	//If there was no '.' then it's a top level identifier, and so the name is the same as the identifier.
	if (m_name == NULL)
	{
		m_name = m_identifier;
	}
}

HostID Entry::GetOwner()
{ 
	return m_owner; 
}

const char* Entry::GetName()
{ 
	return m_name; 
}

const char* Entry::GetIdentifier()
{
	return m_identifier;
}

Entry* Entry::Create(const char* identifier, E_DATA_TYPE type)
{
	if (type == EDT_INT)
	{
		return new IntEntry(0, identifier);
	}
	else if (type == EDT_LONG)
	{
		return new LongEntry(0, identifier);
	}
	else if (type == EDT_BOOLEAN)
	{
		return new BooleanEntry(0, identifier);
	}
	else if (type == EDT_STRING)
	{
		return new StringEntry(0, identifier);
	}

	return NULL;
}

//===============================================================================================
//Int
//===============================================================================================

IntEntry::IntEntry(HostID owner, const char* identifier)
	:Entry(owner, identifier),
	m_integer(0)
{}

void IntEntry::Set(int32_t integer)
{
	m_integer = integer;
}

int32_t IntEntry::Get()
{
	return m_integer;
}

void IntEntry::Serialize(DataBuffer& out)
{
	out.Write<int32_t>(htonl(m_integer));
}

void IntEntry::Deserialize(DataBuffer& in)
{
	int val = 0;
	if (in.Read<int32_t>(val))
	{
		m_integer = ntohl(val);
	}
}

//===============================================================================================
//Long
//===============================================================================================

LongEntry::LongEntry(HostID owner, const char* identifier)
	:Entry(owner, identifier),
	m_integer(0)
{}

void LongEntry::Set(int64_t integer)
{
	m_integer = integer;
}

int64_t LongEntry::Get()
{
	return m_integer;
}

void LongEntry::Serialize(DataBuffer& out)
{
	out.Write<int64_t>(htonll(m_integer));
}

void LongEntry::Deserialize(DataBuffer& in)
{
	int64_t val = 0;
	if (in.Read<int64_t>(val))
	{
		m_integer = ntohll(val);
	}
}

//===============================================================================================
//Float
//===============================================================================================

FloatEntry::FloatEntry(HostID owner, const char* identifier)
	:Entry(owner, identifier),
	m_float(0.0f)
{}

void FloatEntry::Set(float f)
{
	m_float = f;
}

float FloatEntry::Get()
{
	return m_float;
}

void FloatEntry::Serialize(DataBuffer& out)
{
	out.Write<float>(htonf(m_float));
}

void FloatEntry::Deserialize(DataBuffer& in)
{
	float val = 0;
	if (in.Read<float>(val))
	{
		m_float = ntohf(val);
	}
}

//===============================================================================================
//Double
//===============================================================================================

DoubleEntry::DoubleEntry(HostID owner, const char* identifier)
	:Entry(owner, identifier),
	m_double(0.0)
{}

void DoubleEntry::Set(double f)
{
	m_double = f;
}

double DoubleEntry::Get()
{
	return m_double;
}

void DoubleEntry::Serialize(DataBuffer& out)
{
	out.Write<double>(htonll(m_double));
}

void DoubleEntry::Deserialize(DataBuffer& in)
{
	double val = 0;
	if (in.Read<double>(val))
	{
		m_double = ntohll(val);
	}
}

//===============================================================================================
//Boolean
//===============================================================================================

BooleanEntry::BooleanEntry(HostID owner, const char* identifier)
	:Entry(owner, identifier),
	m_boolean(false)
{}

void BooleanEntry::Set(bool b)
{
	m_boolean = b;
}

bool BooleanEntry::Get()
{
	return m_boolean;
}

void BooleanEntry::Serialize(DataBuffer& out)
{
	out.Write<uint8_t>(m_boolean);
}

void BooleanEntry::Deserialize(DataBuffer& in)
{
	uint8_t val = 0;
	if (in.Read<uint8_t>(val))
	{
		m_boolean = (val != 0);
	}
}

//===============================================================================================
//String
//===============================================================================================

StringEntry::StringEntry(HostID owner, const char* identifier)
	:Entry(owner, identifier),
	m_capacity(0),
	m_string(NULL)
{}

StringEntry::~StringEntry()
{
	if (m_string != NULL)
	{
		delete[] m_string;
		m_string = NULL;
	}
}

void StringEntry::Set(const char* str)
{
	size_t length = strlen(str);
	if (length == 0)
	{
		return;
	}

	//Reallocate memory if required
	EnsureCapacity(length);

	//Copy to internal string
	memcpy(m_string, str, length);
}

const char* StringEntry::Get()
{
	return m_string;
}

void StringEntry::Serialize(DataBuffer& out)
{
	uint16_t len = (uint16_t)strlen(m_string);
	out.Write<uint16_t>(len);

	out.Write((const unsigned char*)m_string, len);
}

void StringEntry::Deserialize(DataBuffer& in)
{
	uint16_t len = 0;
	in.Read<uint16_t>(len);

	EnsureCapacity(len);
	in.Read((const unsigned char*)m_string, len);
}

void StringEntry::EnsureCapacity(size_t sz)
{
	if (m_capacity < sz)
	{
		if (m_string != NULL)
		{
			delete[] m_string;
			m_string = NULL;
		}

		m_string = new char[sz];
	}
}

//===============================================================================================
//Unformatted
//===============================================================================================

UnformattedEntry::UnformattedEntry(HostID owner, const char* identifier)
	:Entry(owner, identifier),
	m_capacity(0),
	m_data(NULL)
{}

UnformattedEntry::~UnformattedEntry()
{
	if (m_data != NULL)
	{
		delete[] m_data;
		m_data = NULL;
	}
}

void UnformattedEntry::Set(char* data, size_t dataSz)
{
	if (dataSz == 0)
	{
		return;
	}

	//Reallocate memory if required
	EnsureCapacity(dataSz);

	//Copy to internal string
	memcpy(m_data, data, dataSz);
	m_dataSz = dataSz;
}

char* UnformattedEntry::Get()
{
	return m_data;
}

size_t UnformattedEntry::GetSize()
{
	return m_dataSz;
}

void UnformattedEntry::Serialize(DataBuffer& out)
{
	uint16_t len = (uint16_t)m_dataSz;
	out.Write<uint16_t>(len);

	out.Write((const unsigned char*)m_data, len);
}

void UnformattedEntry::Deserialize(DataBuffer& in)
{
	uint16_t len = 0;
	in.Read<uint16_t>(len);

	EnsureCapacity(len);
	in.Read((const unsigned char*)m_data, len);
	m_dataSz = len;
}

void UnformattedEntry::EnsureCapacity(size_t sz)
{
	if (m_capacity < sz)
	{
		if (m_data != NULL)
		{
			delete[] m_data;
			m_data = NULL;
		}

		m_data = new char[sz];
	}
}