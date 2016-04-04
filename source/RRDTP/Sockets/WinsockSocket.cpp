#include "WinsockSocket.h"

#ifdef RRDTP_PLATFORM_WINDOWS

using namespace rrdtp;

WinsockSocket::WinsockSocket(DataRecievedCallback dataRecievedCallback, void* userPtr, ConnectionAcceptedCallback connectionAcceptedCallback)
	:Socket(dataRecievedCallback, userPtr, connectionAcceptedCallback)
{
	
}
		
WinsockSocket::~WinsockSocket()
{
	
}

static WSADATA* WinsockSocket::m_wsaData = NULL;

SOCKET_ERROR WinsockSocket::commonInit()
{
	if (m_wsaData == NULL)
	{
		m_wsaData = new WSADATA;
		if (WSAStartup(MAKEWORD(2,2), m_wsa) != 0)
		{
			delete m_wsaData;
			m_wsaData = NULL;
			return false;
		}
	}
	
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
	if(m_socket == INVALID_SOCKET)
    {
		WSACleanup();
    }
	
	return true;
}

SOCKET_ERROR WinsockSocket::connect(const char* ip, unsigned int port, SOCKET_PROTOCOL protocol)
{
	if (!commonInit())
	{
		return SE_FAILURE;
	}	
	
	struct sockaddr_in server;
	
	server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
	
	if (connect(m_socket, (struct sockaddr*)&server , sizeof(server)) < 0)
    {
		return;//return FAILURE;
	}
	
	return SUCCESS;
}

SOCKET_ERROR WinsockSocket::listen(unsigned int port, SOCKET_PROTOCOL protocol)
{
	if (!commonInit())
	{
		return;// FAILURE;
	}	
	
	struct sockaddr_in server;
	
	server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
	
	if (connect(m_socket, (struct sockaddr*)&server , sizeof(server)) < 0)
    {
		return;// FAILURE;
	}
	
	//return SUCCESS;
}

void WinsockSocket::close()
{
	WSACleanup();
}

size_t WinsockSocket::send(const void* data, size_t sz)
{
	
}

#endif