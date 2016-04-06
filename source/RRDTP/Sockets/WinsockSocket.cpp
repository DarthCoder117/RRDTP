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

WSADATA* WinsockSocket::m_wsaData = NULL;

E_SOCKET_ERROR WinsockSocket::CommonInit()
{
	if (m_wsaData == NULL)
	{
		m_wsaData = new WSADATA;
		if (WSAStartup(MAKEWORD(2,2), m_wsaData) != 0)
		{
			delete m_wsaData;
			m_wsaData = NULL;
			return ESE_FAILURE;
		}
	}
	
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
	if(m_socket == INVALID_SOCKET)
    {
		return ESE_FAILURE;
    }
	
	return ESE_SUCCESS;
}

E_SOCKET_ERROR WinsockSocket::Connect(const char* ip, unsigned int port, E_SOCKET_PROTOCOL protocol)
{
	if (CommonInit() != ESE_SUCCESS)
	{
		return ESE_FAILURE;
	}	
	
	struct sockaddr_in server;
	
	server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
	
	if (connect(m_socket, (struct sockaddr*)&server , sizeof(server)) < 0)
    {
		return ESE_FAILURE;
	}
	
	return ESE_SUCCESS;
}

E_SOCKET_ERROR WinsockSocket::Listen(unsigned int port, E_SOCKET_PROTOCOL protocol)
{
	if (CommonInit() != ESE_SUCCESS)
	{
		return ESE_FAILURE;
	}	
	
	struct sockaddr_in server;
	
	server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
	
	if (connect(m_socket, (struct sockaddr*)&server , sizeof(server)) < 0)
    {
		return ESE_FAILURE;
	}
	
	listen(m_socket, 3);
	
	return ESE_SUCCESS;
}

E_SOCKET_ERROR WinsockSocket::Accept()
{
	SOCKET newSocket;
	struct sockaddr_in client;
	int c;

	newSocket = accept(m_socket, (struct sockaddr *)&client, &c);
    if (newSocket == INVALID_SOCKET)
    {
        return ESE_FAILURE;
    }
	
	return ESE_SUCCESS;
}

void WinsockSocket::Close()
{
	closesocket(m_socket);
	m_socket = NULL;
}

size_t WinsockSocket::Send(const void* data, size_t sz)
{
	return send(m_socket, (const char*)data, sz, 0);
}

void WinsockSocket::Poll()
{
	char data[2000];
	size_t recievedDataSz = recv(m_socket, data, 2000, 0);
	
	if (m_dataRecievedCallback != NULL)
	{
		m_dataRecievedCallback((void*)data, recievedDataSz);
	}
}

#endif