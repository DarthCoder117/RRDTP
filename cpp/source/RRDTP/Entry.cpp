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

void IntEntry::Set(int integer)
{
	m_integer = integer;
}

int IntEntry::Get()
{
	return m_integer;
}

void IntEntry::Serialize(DataBuffer& out)
{
	out.Write<int>(htonl(m_integer));
}

void IntEntry::Deserialize(DataBuffer& in)
{
	int val = 0;
	if (in.Read<int>(val))
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

void LongEntry::Set(long integer)
{
	m_integer = integer;
}

long LongEntry::Get()
{
	return m_integer;
}

void LongEntry::Serialize(DataBuffer& out)
{
	out.Write<long>(htonl(m_integer));
}

void LongEntry::Deserialize(DataBuffer& in)
{
	long val = 0;
	if (in.Read<long>(val))
	{
		m_integer = ntohl(val);
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
	out.Write<char>(m_boolean);
}

void BooleanEntry::Deserialize(DataBuffer& in)
{
	char val = 0;
	if (in.Read<char>(val))
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
	short len = (unsigned short)strlen(m_string);
	out.Write<unsigned short>(len);

	out.Write((const unsigned char*)m_string, len);
}

void StringEntry::Deserialize(DataBuffer& in)
{
	unsigned short len = 0;
	in.Read<unsigned short>(len);

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