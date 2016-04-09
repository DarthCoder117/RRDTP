#include "RRDTP/Connection.h"
#include <sstream>

using namespace rrdtp;

void dataRecieved(rrdtp::Socket* self, rrdtp::HostID sender, void* data, size_t dataSz)
{
	Connection* connection = (Connection*)self->GetUserData();


}

Connection::Connection()
	:m_socket(NULL)
{
	m_socket = Socket::Create(&dataRecieved, this);
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

void Connection::StartClient(const char* ip, unsigned int port)
{
	if (m_socket != NULL)
	{
		m_socket->Connect(ip, port, ESP_TCP);
	}
}

void Connection::SetInt(const char* identifier, int val)
{
	if (m_socket != NULL)
	{
		/*PacketHeader header;
		header.m_eventType = htonl(EET_VALUE_UPDATE);

		std::stringstream s;
		s.write((const char*)&header, sizeof(header));//Write header
		s << identifier;//Write value identifier
		s.write()

		if (m_socket->IsServer())
		{
			//TODO: Send value to all connected clients
		}
		else
		{
			//TODO: Send value to server
		}

		m_localDataStore.SetInt(identifier, val);*/
	}
}

int Connection::GetInt(const char* identifier, int defaultVal)
{
	return m_localDataStore.GetInt(identifier, defaultVal);
}

void Connection::Poll()
{
	if (m_socket != NULL)
	{
		m_socket->Accept();
		m_socket->Poll();
	}
}