#include "RRDTP/Connection.h"
#include "RRDTP/Platform.h"
#include "RRDTP/DataBuffer.h"
#include "RRDTP/Entry.h"
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
		if (eventType == EET_SET || eventType == EET_DELETE)
		{
			//SET and DELETE both start with the value identifier, so we read that here.

			//Length of value identifier
			char identifierLength = 0;
			buffer.Read<char>(identifierLength);

			//Value identifier string.
			char identifier[255];
			buffer.Read((unsigned char*)identifier, identifierLength);
			identifier[identifierLength] = '\0';

			if (eventType == EET_SET)
			{
				//Data type
				char dataType = 0;
				buffer.Read<char>(dataType);

				//Create/retrieve the entry and let it deserialize itself.
				Entry* entry = connection->m_localDataStore.Create(sender, identifier, (E_DATA_TYPE)dataType);
				if (entry != NULL && entry->GetType() == dataType)
				{
					entry->Deserialize(buffer);

					//Trigger value change callback
					if (connection->m_valueChangedCallback)
					{
						connection->m_valueChangedCallback(connection, entry);
					}
				}
			}
			else if(eventType == EET_DELETE)
			{
				connection->m_localDataStore.Delete(identifier);
			}
		}
	}
}

void Connection::clientConnected(Socket* self, HostID client)
{
	Connection* connection = (Connection*)self->GetUserData();

	//Send out update packet for all currently existing values.
	connection->SynchronizeAllEntries(client);
}

Connection::Connection()
	:m_socket(NULL),
	m_valueChangedCallback(NULL)
{
	m_socket = Socket::Create(&Connection::dataRecieved, this, &Connection::clientConnected);
}

Connection::~Connection()
{
	Close();
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

			return true;
		}
	}

	return false;
}

void Connection::Close()
{
	if (m_socket != NULL)
	{
		m_socket->Close();
		delete m_socket;
		m_socket = NULL;
	}
}

void Connection::SendUpdatePacket(Entry* entry, HostID target)
{
	if (m_socket != NULL && entry != NULL)
	{	
		int identifierLength = strlen(entry->GetIdentifier());
		if (identifierLength >= 255)
		{
			return;
		}

		unsigned char* packetData = new unsigned char[512];
		DataBuffer buffer(packetData, 512);

		//Write header for set packet
		buffer.Write<char>(1);//Protocol version
		buffer.Write<char>(EET_SET);//Event type

		buffer.Write<char>((char)identifierLength);//Value identifier length
		buffer.Write((const unsigned char*)entry->GetIdentifier(), identifierLength);//Value identifier

		//Data type
		buffer.Write<char>((char)entry->GetType());

		//Let the entry write its data. 
		//The entry will handle endianness conversion and tracking of data size.
		entry->Serialize(buffer);

		//Send update packet to newly connected clients.
		if (target == -1)
		{
			m_socket->SendAll(packetData, buffer.GetPosition());
		}
		else
		{
			m_socket->Send(packetData, buffer.GetPosition(), target);
		}

		delete[] packetData;
	}
}

void Connection::SendDeletionPacket(const char* identifier)
{
	int identifierLength = strlen(identifier);
	if (identifierLength >= 255)
	{
		return;
	}

	unsigned char* packetData = new unsigned char[512];
	DataBuffer buffer(packetData, 512);

	//Write header for set packet
	buffer.Write<char>(1);//Protocol version
	buffer.Write<char>(EET_DELETE);//Event type

	buffer.Write<char>((char)identifierLength);//Value identifier length
	buffer.Write((const unsigned char*)identifier, identifierLength);//Value identifier

	m_socket->SendAll(packetData, buffer.GetPosition());

	delete[] packetData;
}

void Connection::SynchronizeAllEntries(HostID client)
{
	SynchronizeAllEntriesImplementation(m_localDataStore.GetRootCategory(), client);
}

void Connection::SynchronizeAllEntriesImplementation(Category* start, HostID client)
{
	//Send packet for each entry
	List<Entry*>::Node* n = start->GetEntries().Begin();
	while (n != NULL)
	{
		SendUpdatePacket(n->GetValue(), client);

		n = n->GetNext();
	}
}

void Connection::SetInt(const char* identifier, int32_t val)
{
	//Create/retrieve entry (id doesn't matter locally).
	IntEntry* entry = (IntEntry*)m_localDataStore.Create(-1, identifier, EDT_INT);
	if (entry != NULL)
	{
		//Update the entry
		entry->Set(val);

		SendUpdatePacket(entry);//TODO: Track changed entries and only send those in Poll
	}
}

int32_t Connection::GetInt(const char* identifier, int32_t defaultVal)
{
	//Retrieve entry
	Entry* entry = m_localDataStore.Get(identifier);
	if (entry != NULL && entry->GetType() == EDT_INT)
	{
		//Return the value
		return ((IntEntry*)entry)->Get();
	}

	return defaultVal;
}

void Connection::SetLong(const char* identifier, int64_t val)
{
	//Create/retrieve entry (id doesn't matter locally).
	LongEntry* entry = (LongEntry*)m_localDataStore.Create(-1, identifier, EDT_LONG);
	if (entry != NULL)
	{
		//Update the entry
		entry->Set(val);

		SendUpdatePacket(entry);
	}
}

int64_t Connection::GetLong(const char* identifier, int64_t defaultVal)
{
	//Retrieve entry
	Entry* entry = m_localDataStore.Get(identifier);
	if (entry != NULL && entry->GetType() == EDT_LONG)
	{
		//Return the value
		return ((LongEntry*)entry)->Get();
	}

	return defaultVal;
}

void Connection::SetBool(const char* identifier, bool val)
{
	//Create/retrieve entry (id doesn't matter locally).
	BooleanEntry* entry = (BooleanEntry*)m_localDataStore.Create(-1, identifier, EDT_BOOLEAN);
	if (entry != NULL)
	{
		//Update the entry
		entry->Set(val);

		SendUpdatePacket(entry);
	}
}

bool Connection::GetBool(const char* identifier, bool defaultVal)
{
	//Retrieve entry
	Entry* entry = m_localDataStore.Get(identifier);
	if (entry != NULL && entry->GetType() == EDT_BOOLEAN)
	{
		//Return the value
		return ((BooleanEntry*)entry)->Get();
	}

	return defaultVal;
}

void Connection::SetString(const char* identifier, const char* str)
{
	int stringLength = strlen(str);

	//Create/retrieve entry (id doesn't matter locally).
	StringEntry* entry = (StringEntry*)m_localDataStore.Create(-1, identifier, EDT_STRING);
	if (entry != NULL)
	{
		//Update the entry
		entry->Set(str);

		SendUpdatePacket(entry);
	}
}

const char* Connection::GetString(const char* identifier, const char* defaultVal)
{
	//Retrieve entry
	Entry* entry = m_localDataStore.Get(identifier);
	if (entry != NULL && entry->GetType() == EDT_STRING)
	{
		//Return the value
		return ((StringEntry*)entry)->Get();
	}

	return defaultVal;
}

Entry* Connection::GetEntry(const char* identifier)
{
	return m_localDataStore.Get(identifier);
}

Category* Connection::GetCategory(const char* identifier)
{
	return NULL;//TODO: Search for category
}

void Connection::Delete(const char* identifier)
{
	if (m_localDataStore.Delete(identifier))
	{
		SendDeletionPacket(identifier);
	}
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