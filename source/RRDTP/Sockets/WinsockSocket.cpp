#include "WinsockSocket.h"

#ifdef RRDTP_PLATFORM_WINDOWS

using namespace rrdtp;

WinsockSocket::WinsockSocket(DataRecievedCallback dataRecievedCallback, void* userPtr, ConnectionAcceptedCallback connectionAcceptedCallback)
	:Socket(dataRecievedCallback, userPtr, connectionAcceptedCallback),
	m_isServer(false)
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

E_SOCKET_ERROR WinsockSocket::CommonInit(E_SOCKET_PROTOCOL protocol)
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

	int p = IPPROTO_TCP;
	if (protocol == ESP_UDP)
	{
		p = IPPROTO_UDP;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, p);
	if(m_socket == INVALID_SOCKET)
    {
		DWORD lastErr = GetLastError();
		return ESE_FAILURE;
    }
	
	return ESE_SUCCESS;
}

E_SOCKET_ERROR WinsockSocket::Connect(const char* ip, unsigned int port, E_SOCKET_PROTOCOL protocol)
{
	if (CommonInit(protocol) != ESE_SUCCESS)
	{
		return ESE_FAILURE;
	}	
	
	struct sockaddr_in server;
	
	server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
	
	int err = connect(m_socket, (struct sockaddr*)&server, sizeof(server));
	if (err < 0)
    {
		return ESE_FAILURE;
	}

	//Set to non-blocking mode
	u_long iMode = 1;
	ioctlsocket(m_socket, FIONBIO, &iMode);
	
	m_isServer = false;
	return ESE_SUCCESS;
}

E_SOCKET_ERROR WinsockSocket::Listen(unsigned int port, E_SOCKET_PROTOCOL protocol)
{
	if (CommonInit(protocol) != ESE_SUCCESS)
	{
		return ESE_FAILURE;
	}	
	
	struct sockaddr_in server;
	
	server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
	
	int err = bind(m_socket, (struct sockaddr*)&server, sizeof(server));
	if (err != 0)
    {
		return ESE_FAILURE;
	}
	
	err = listen(m_socket, 3);
	if (err != 0)
	{
		return ESE_FAILURE;
	}
	
	m_isServer = true;
	return ESE_SUCCESS;
}

bool WinsockSocket::IsServer()
{
	return m_isServer;
}

HostID WinsockSocket::Accept(E_SOCKET_ERROR* errorCodeOut)
{
	SOCKET newSocket;
	struct sockaddr_in client;
	int c = sizeof(client);

	newSocket = accept(m_socket, (struct sockaddr *)&client, &c);
    if (newSocket == INVALID_SOCKET)
    {
		if (errorCodeOut)
		{
			*errorCodeOut = ESE_FAILURE;
		}
		return -1;
    }
	
	if (errorCodeOut)
	{
		*errorCodeOut = ESE_SUCCESS;
	}

	m_connectedClients.push_back(newSocket);

	if (m_connectionAcceptedCallback != NULL)
	{
		m_connectionAcceptedCallback(this, newSocket);
	}

	return (size_t)newSocket;
}

void WinsockSocket::Close()
{
	std::list<SOCKET>::iterator iter;
	for (iter = m_connectedClients.begin(); iter != m_connectedClients.end(); ++iter)
	{
		shutdown(*iter, SD_SEND);
		closesocket(*iter);
	}

	shutdown(m_socket, SD_SEND);
	closesocket(m_socket);
	m_socket = NULL;
}

size_t WinsockSocket::Send(const void* data, size_t sz, HostID host)
{
	if (m_isServer)//The server needs a host ID specified to send the data to.
	{
		return send((SOCKET)host, (const char*)data, sz, 0);
	}
	
	return send(m_socket, (const char*)data, sz, 0);
}

void WinsockSocket::Poll()
{
	char data[2000];

	//Server socket reads data from all connected client sockets
	if (m_isServer)
	{
		std::list<SOCKET>::iterator iter;
		for (iter = m_connectedClients.begin(); iter != m_connectedClients.end(); ++iter)
		{
			int recievedDataSz = recv(*iter, data, 2000, 0);
			if (recievedDataSz != SOCKET_ERROR)
			{
				if (m_dataRecievedCallback != NULL)
				{
					m_dataRecievedCallback(this, *iter, (void*)data, recievedDataSz);
				}
			}
		}
	}
	//Client socket only needs to read from itself
	else
	{
		int recievedDataSz = recv(m_socket, data, 2000, 0);

		int lastError = WSAGetLastError();
		if (lastError != WSAEWOULDBLOCK)
		{
			if (recievedDataSz != SOCKET_ERROR)
			{
				//Data was recieved, so trigger the callback.
				if (m_dataRecievedCallback != NULL)
				{
					m_dataRecievedCallback(this, m_socket, (void*)data, recievedDataSz);
				}
			}
			else
			{
				//Otherwise the client was disconnected, so trigger that callback.
				//TODO: Disconnect callback.
			}
		}
	}
}

#endif