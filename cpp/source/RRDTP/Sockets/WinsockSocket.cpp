/*==============================================================================
The MIT License (MIT)

Copyright (c) 2016 Tanner Mickelson

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
	
	//Make server non-blocking.
	u_long iMode = 1;
	ioctlsocket(m_socket, FIONBIO, &iMode);

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
	if (IsServer())
	{
		std::list<SOCKET>::iterator iter;
		for (iter = m_connectedClients.begin(); iter != m_connectedClients.end(); ++iter)
		{
			shutdown(*iter, SD_SEND);
			closesocket(*iter);
		}

		m_connectedClients.clear();
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

void WinsockSocket::SendAll(const void* data, size_t sz)
{
	if (m_isServer)//The server needs a host ID specified to send the data to.
	{
		std::list<SOCKET>::iterator iter;
		for (iter = m_connectedClients.begin(); iter != m_connectedClients.end(); ++iter)
		{
			send(*iter, (const char*)data, sz, 0);
		}
	}
	else
	{
		send(m_socket, (const char*)data, sz, 0);
	}
}

void WinsockSocket::Poll()
{
	if (m_socket != NULL)
	{
		if (m_isServer)
		{
			PollServer();
		}
		else
		{
			PollClient();
		}
	}
}

void WinsockSocket::PollServer()
{
	char data[2000];

	//Clear out the set before polling.
	FD_ZERO(&m_readFDS);

	//Add server socket to set
	//FD_SET(m_socket, &m_readFDS);
	
	//Add client sockets
	std::list<SOCKET>::iterator iter;
	for (iter = m_connectedClients.begin(); iter != m_connectedClients.end(); ++iter)
	{
		FD_SET(*iter, &m_readFDS);
	}

	//Check to see if any sockets can be read from.
	TIMEVAL timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	int numReady = select(0, &m_readFDS, NULL, NULL, &timeout);
	if (numReady != SOCKET_ERROR && numReady > 0)
	{
		//Iterate over each client socket and check for I/O events.
		std::list<SOCKET>::iterator iter = m_connectedClients.begin();
		while (iter != m_connectedClients.end())
		{
			//If the fds isn't set then there's no I/O event on this socket and we can move on.
			if (!FD_ISSET(*iter, &m_readFDS))
			{
				continue;
			}

			//Otherwise there's data to be recieved.
			int recievedDataSz = recv(*iter, data, 2000, 0);

			int lastError = WSAGetLastError();
			if (lastError != WSAEWOULDBLOCK)
			{
				if (recievedDataSz != SOCKET_ERROR)
				{
					if (m_dataRecievedCallback != NULL)
					{
						m_dataRecievedCallback(this, *iter, (void*)data, recievedDataSz);
					}

					++iter;
				}
				else
				{
					//If we get here, then the client was disconnected, so close it and trigger the disconnection callback.
					shutdown(*iter, SD_SEND);
					closesocket(*iter);
					iter = m_connectedClients.erase(iter);
					//TODO: Disconnect callback.
					printf("Client disconnected.\n");
				}
			}
		}
	}
}

void WinsockSocket::PollClient()
{
	char data[2000];

	//Clear out the set before polling.
	FD_ZERO(&m_readFDS);

	//Add socket to set
	FD_SET(m_socket, &m_readFDS);

	TIMEVAL timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	int numReady = select(0, &m_readFDS, NULL, NULL, &timeout);
	if (numReady != SOCKET_ERROR && numReady > 0)
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
				//Otherwise the server has disconnected, so close the socket and trigger the disconnection callback.
				Close();
				//TODO: Disconnect callback.
				printf("Server disconnected.\n");
			}
		}
	}
}

#endif