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
#include "RRDTP/Sockets/BSDSocket.h"
#if defined(RRDTP_PLATFORM_UNIX)

using namespace rrdtp;

static int portNumber = 8080;
static int m_SocketFD;


BSDSocket::BSDSocket(DataRecievedCallback dataRecievedCallback, void* userPtr, ConnectionAcceptedCallback connectionAcceptedCallback)
    :Socket(dataRecievedCallback, userPtr, connectionAcceptedCallback),
	m_isServer(false),
	m_socket(-1)

{
   
}

BSDSocket::~BSDSocket()
{

}

E_SOCKET_ERROR BSDSocket::CommonInit(E_SOCKET_PROTOCOL protocol)
{
	int p = IPPROTO_TCP;
	if (protocol == ESP_UDP)
	{
		p = IPPROTO_UDP;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, p);
	perror("Socket");
	if (m_socket == -1)
	{
		return ESE_FAILURE;
	}

	return ESE_SUCCESS;
}

E_SOCKET_ERROR BSDSocket::Connect(const char *ip, unsigned int port, E_SOCKET_PROTOCOL protocol)
{
	if (CommonInit(protocol) != ESE_SUCCESS)
	{
		return ESE_FAILURE;
	}

	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	int err = connect(m_socket, (struct sockaddr*)&server, sizeof(server));
	perror("Connect");
	if (err < 0)
	{
		return ESE_FAILURE;
	}

	m_isServer = false;
	return ESE_SUCCESS;
}
#include <iostream>
E_SOCKET_ERROR BSDSocket::Listen(unsigned int port, E_SOCKET_PROTOCOL protocol)
{
	if (CommonInit(protocol) != ESE_SUCCESS)
	{
		return ESE_FAILURE;
	}
	
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = port;

	int res = bind(m_socket, (struct sockaddr*)&server, sizeof(server));
	perror("Bind");
	if (res < 0)
	{
		close(m_socket);
		return ESE_FAILURE;
	}

	res = listen(m_socket, 3);
	perror("Listen");
	if (res < 0)
	{
		return ESE_FAILURE;
	}

	m_isServer = true;
	return ESE_SUCCESS;
}

bool BSDSocket ::IsServer()
{
    return m_isServer;
}

HostID BSDSocket::Accept(E_SOCKET_ERROR *errorCodeOut)
{
	int newSocket;
	struct sockaddr_in client;
	int c = sizeof(client);

	newSocket = accept(m_socket, (struct sockaddr*)&client, (socklen_t*)&c);
	if (newSocket < 0)
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

	return (HostID)newSocket;
}

void BSDSocket::Close()
{
	if (IsServer())
	{
		std::list<int>::iterator iter;
		for (iter = m_connectedClients.begin(); iter != m_connectedClients.end(); ++iter)
		{
			shutdown(*iter, SHUT_RDWR);
			close(*iter);
		}

		m_connectedClients.clear();
	}

	shutdown(m_socket, SHUT_RDWR);
	close(m_socket);
	m_socket = -1;
}

size_t BSDSocket::Send(const void* data, size_t sz, HostID host)
{
	if (m_isServer)//The server needs a host ID specified to send the data to.
	{
		return send((int)host, (const void*)data, sz, 0);
	}

	return send(m_socket, (const void*)data, sz, 0);
}
		
void BSDSocket::SendAll(const void* data, size_t sz)
{
	if (m_isServer)//The server needs a host ID specified to send the data to.
	{
		std::list<int>::iterator iter;
		for (iter = m_connectedClients.begin(); iter != m_connectedClients.end(); ++iter)
		{
			send(*iter, (const void*)data, sz, 0);
		}
	}
	else
	{
		send(m_socket, (const void*)data, sz, 0);
	}
}

void BSDSocket::Poll() 
{
	if (m_socket != -1)
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

void BSDSocket::PollServer()
{
	char data[2000];

	//Clear out the set before polling.
	FD_ZERO(&m_readFDS);

	//Add server socket to set
	//FD_SET(m_socket, &m_readFDS);
	int maxSd = 0;

	//Add client sockets
	std::list<int>::iterator iter;
	for (iter = m_connectedClients.begin(); iter != m_connectedClients.end(); ++iter)
	{
		int sd = *iter;

		FD_SET(sd, &m_readFDS);

		if (sd > maxSd)
		{
			maxSd = sd;
		}
	}

	//Check to see if any sockets can be read from.
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	int numReady = select(maxSd+1, &m_readFDS, NULL, NULL, &timeout);
	if (numReady != -1 && numReady > 0)
	{
		//Iterate over each client socket and check for I/O events.
		std::list<int>::iterator iter = m_connectedClients.begin();
		while (iter != m_connectedClients.end())
		{
			//If the fds isn't set then there's no I/O event on this socket and we can move on.
			if (!FD_ISSET(*iter, &m_readFDS))
			{
				continue;
			}

			//Otherwise there's data to be recieved.
			int recievedDataSz = recv(*iter, data, 2000, 0);

			if (recievedDataSz != -1)
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
				shutdown(*iter, SHUT_RDWR);
				close(*iter);
				iter = m_connectedClients.erase(iter);
				//TODO: Disconnect callback.
				printf("Client disconnected.\n");
			}
		}
	}
}

void BSDSocket::PollClient()
{
	char data[2000];

	//Clear out the set before polling.
	FD_ZERO(&m_readFDS);

	//Add socket to set
	FD_SET(m_socket, &m_readFDS);

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	int numReady = select(0, &m_readFDS, NULL, NULL, &timeout);
	if (numReady != -1 && numReady > 0)
	{
		int recievedDataSz = recv(m_socket, data, 2000, 0);

		if (recievedDataSz != -1)
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

#endif
