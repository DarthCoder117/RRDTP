#include "WinsockSocket.h"

#ifdef RRDTP_PLATFORM_WINDOWS

using namespace rrdtp;

WinsockSocket::WinsockSocket(DataRecievedCallback dataRecievedCallback, void* userPtr, ConnectionAcceptedCallback connectionAcceptedCallback)
	:Socket(dataRecievedCallback, userPtr, connectionAcceptedCallback)
{
	
}
		
WinsockSocket::~WinsockSocket()
{
	if (m_wsaData != NULL)
	{
		WSACleanup();
		m_wsaData = NULL;
	}
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
			return SE_ERROR;
		}
	}
	
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
	if(m_socket == INVALID_SOCKET)
    {
		return SE_ERROR;
    }
	
	return SE_SUCCESS;
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
		return SE_FAILURE;
	}
	
	return SE_SUCCESS;
}

SOCKET_ERROR WinsockSocket::listen(unsigned int port, SOCKET_PROTOCOL protocol)
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
		return SE_FAILURE;
	}
	
	listen(m_socket, 3);
	
	return SE_SUCCESS;
}

SOCKET_ERROR WinsockSocket::accept()
{
	SOCKET newSocket;
	newSocket = accept(s , (struct sockaddr *)&client, &c);
    if (newSocket == INVALID_SOCKET)
    {
        return SE_FAILURE;
    }
	
	return SE_SUCCESS;
}

void WinsockSocket::close()
{
	closesocket(m_socket);
}

size_t WinsockSocket::send(const void* data, size_t sz)
{
	return send(m_socket, data, sz, 0);
}

void WinsockSocket::poll()
{
	char* data[2000];
	size_t recievedDataSz = recv(m_socket, data, 2000);
	
	if (m_dataRecievedCallback)
	{
		m_dataRecievedCallback((void*)data, recievedDataSz);
	}
}

#endif