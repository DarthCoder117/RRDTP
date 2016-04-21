#include "RRDTP/Connection.h"
#include "RRDTP/Platform.h"
#include "RRDTP/DataBuffer.h"
#include <sstream>
#include <cinttypes>
#ifdef RRDTP_PLATFORM_WINDOWS
#include <Winsock2.h>
#endif

using namespace rrdtp;

void Connection::dataRecieved(Socket* self, HostID sender, void* data, size_t dataSz)
{
	Connection* connection = (Connection*)self->GetUserData();

	if (data != NULL && dataSz > 0)
	{
		DataBuffer buffer((unsigned char*)data, dataSz);

		//Protocol version and event type are common to all packets.
		char protocolVersion = 0;
		buffer.Read<char>(protocolVersion);

		char eventType = 0;
		buffer.Read<char>(eventType);

		//Depending on the event type, different packets may have different contents.
		if (eventType == EET_UPDATE || eventType == EET_CREATE)
		{
			//Length of value identifier
			char identifierLength = 0;
			buffer.Read<char>(identifierLength);

			//Value identifier string.
			char identifier[255];
			buffer.Read((unsigned char*)identifier, identifierLength);
			identifier[identifierLength] = '\0';

			//Data type
			char dataType = 0;
			buffer.Read<char>(dataType);

			//Size of data
			short dataSz = 0;
			buffer.Read<short>(dataSz);
			dataSz = ntohs(dataSz);

			//Actual data
			unsigned char* value = ((unsigned char*)data)+buffer.GetPosition();

			if (eventType == EET_CREATE)
			{
				Entry* entry = connection->m_localDataStore.Create(sender, identifier, (E_DATA_TYPE)dataType);
				if (entry != NULL)
				{
					entry->Set(value, dataSz);
				}
			}
			else
			{
				Entry* entry = connection->m_localDataStore.Get(identifier);
				if (entry != NULL && /*entry->GetOwner() == sender &&*/ entry->GetType() == dataType)
				{
					entry->Set(value, dataSz);
				}
			}
		}	
	}
}

void Connection::clientConnected(Socket* self, HostID client)
{
	Connection* connection = (Connection*)self->GetUserData();

	//Once connected to the server, send it the client handshake packet.
	unsigned char packetData[2];
	DataBuffer buffer(packetData, 2);

	buffer.Write<char>(1);//Protocol version
	buffer.Write<char>(EET_SERVER_HANDSHAKE);//Event type

	self->Send(packetData, 2);

	//TODO: Iterate through all values in local store and create them on the newly connected client.
}

Connection::Connection()
	:m_socket(NULL)
{
	m_socket = Socket::Create(&Connection::dataRecieved, this, &Connection::clientConnected);
}

Connection::~Connection()
{
	if (m_socket != NULL)
	{
		delete m_socket;
	}
}

void Connection::StartServer(unsigned int port)
{
	if (m_socket != NULL)
	{
		m_socket->Listen(port, ESP_TCP);
	}
}

bool Connection::StartClient(const char* ip, unsigned int port)
{
	if (m_socket != NULL)
	{
		if (m_socket->Connect(ip, port, ESP_TCP) == ESE_SUCCESS)
		{
			printf("Connected to server.\n");

			//Once connected to the server, send it the client handshake packet.
			unsigned char packetData[2];
			DataBuffer buffer(packetData, 2);

			buffer.Write<char>(1);//Protocol version
			buffer.Write<char>(EET_CLIENT_HANDSHAKE);//Event type

			m_socket->Send(packetData, 2);

			return true;
		}
	}

	return false;
}

void Connection::SendUpdatePacket(E_DATA_TYPE type, const char* identifier, const unsigned char* data, short dataSz)
{
	if (m_socket != NULL && identifier != NULL && data != NULL && dataSz > 0)
	{	
		int identifierLength = strlen(identifier);
		if (identifierLength >= 255)
		{
			return;
		}

		unsigned char* packetData = new unsigned char[512];
		DataBuffer buffer(packetData, 512);

		buffer.Write<char>(1);//Protocol version
		buffer.Write<char>(EET_UPDATE);//Event type

		buffer.Write<char>((char)identifierLength);//Value identifier length
		buffer.Write((const unsigned char*)identifier, identifierLength);//Value identifier

		buffer.Write<char>((char)type);//Data type
		buffer.Write<short>(htons(dataSz));//Size of data

		buffer.Write(data, dataSz);//Write the actual data. Conversion to network byte order should have been done already.

		//Send update packet to all connected clients.
		m_socket->SendAll(packetData, buffer.GetPosition());

		delete[] packetData;
	}
}

void Connection::SetInt(const char* identifier, int val)
{
	if (m_socket != NULL)
	{
		//Create/retrieve entry (id doesn't matter locally).
		Entry* entry = m_localDataStore.Create(-1, identifier, EDT_INT);
		if (entry != NULL)
		{
			//Update the entry
			entry->Set<int>(val);

			//Send the update packet after converting to network byte order
			int value = htonl(val);
			SendUpdatePacket(EDT_INT, identifier, (const unsigned char*)&value, sizeof(int));
		}
	}
}

int Connection::GetInt(const char* identifier, int defaultVal)
{
	if (m_socket != NULL)
	{
		//Retrieve entry
		Entry* entry = m_localDataStore.Get(identifier);
		if (entry != NULL && entry->GetType() == EDT_INT)
		{
			//Return the value
			return entry->Get<int>();
		}
	}

	return defaultVal;
}

void Connection::SetLong(const char* identifier, long val)
{
	if (m_socket != NULL)
	{
		//Create/retrieve entry (id doesn't matter locally).
		Entry* entry = m_localDataStore.Create(-1, identifier, EDT_LONG);
		if (entry != NULL)
		{
			//Update the entry
			entry->Set<long>(val);

			//Send the update packet after converting to network byte order
			long value = htonl(val);
			SendUpdatePacket(EDT_LONG, identifier, (const unsigned char*)&value, sizeof(long));
		}
	}
}

long Connection::GetLong(const char* identifier, long defaultVal)
{
	if (m_socket != NULL)
	{
		//Retrieve entry
		Entry* entry = m_localDataStore.Get(identifier);
		if (entry != NULL && entry->GetType() == EDT_LONG)
		{
			//Return the value
			return entry->Get<long>();
		}
	}

	return defaultVal;
}

void Connection::SetBool(const char* identifier, bool val)
{
	if (m_socket != NULL)
	{
		//Create/retrieve entry (id doesn't matter locally).
		Entry* entry = m_localDataStore.Create(-1, identifier, EDT_BOOLEAN);
		if (entry != NULL)
		{
			//Update the entry
			entry->Set<bool>(val);

			//Send as char to avoid having to deal with byte-ordering
			char value = (char)val;
			SendUpdatePacket(EDT_BOOLEAN, identifier, (const unsigned char*)&value, sizeof(char));
		}
	}
}

bool Connection::GetBool(const char* identifier, bool defaultVal)
{
	if (m_socket != NULL)
	{
		//Retrieve entry
		Entry* entry = m_localDataStore.Get(identifier);
		if (entry != NULL && entry->GetType() == EDT_BOOLEAN)
		{
			//Return the value
			return entry->Get<bool>();
		}
	}

	return defaultVal;
}

void Connection::Poll()
{
	if (m_socket != NULL)
	{
		if (m_socket->IsServer())
		{
			if (m_socket->Accept() != -1)
			{
				printf("Accepted connection.\n");
			}
		}

		m_socket->Poll();
	}
}