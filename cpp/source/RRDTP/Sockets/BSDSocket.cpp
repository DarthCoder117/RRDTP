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
	m_isServer(false)

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

	m_SocketFD = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, p);
    if (m_SocketFD == -1)
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

	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	int err = connect(m_socket, (struct sockaddr*)&server, sizeof(server));
	if (err < 0)
	{
		return ESE_FAILURE;
	}

	m_isServer = false;
	return ESE_SUCCESS;
}

E_SOCKET_ERROR BSDSocket::Listen(unsigned int port, E_SOCKET_PROTOCOL protocol)
{
	if (CommonInit(protocol) != ESE_SUCCESS)
	{
		return ESE_FAILURE;
	}
	
	struct sockaddr_in server;

	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

    if (bind(m_SocketFD, (struct sockaddr*)&server, sizeof(struct server) < 0))
    {
        close(m_SocketFD);
		return ESE_FAILURE;
    }

	if (listen(m_socket, 3) < 0)
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

        newSocket = accept(m_SocketFD, (struct sockaddr *)&client, (socklen_t *) &c);

        if (newSocket < 0)
        {
            if (errorCodeOut)
            {
                *errorCodeOut = ESE_FAILURE;
            }
            return -1;
        }

        if(errorCodeOut)
        {
            *errorCodeOut = ESE_SUCCESS;
        }


        return size_t(newSocket);




    }

    HostID BSDSocket::Write(const void *data, size_t sz, HostID hostID)
    {

        if (m_isServer)
        {
            return write(hostID, (const char*)data, sz);
        }

        return write(m_SocketFD, (const char*)data, sz);


    }

HostID BSDSocket :: Read(const void *data, size_t sz, HostID hostID)

{
    int readCount = 0;
    int count = 0;
    int numToRead = 32;



    while(count < numToRead)
    {
        if(readCount == read(m_SocketFD, (void *) data, sz - hostID))
        {
            count += readCount;
            data += readCount;

        }
        else if(readCount < 0)
        {
            close(m_SocketFD);
            exit(EXIT_FAILURE);
        }

        printf("Numbers of bytes read: %d " , count );


    }




}


size_t BSDSocket::Send(const void* data, size_t sz, HostID host)
{
	return 0;
}
		
void BSDSocket::SendAll(const void* data, size_t sz)
{

}


    void BSDSocket :: Close()
    {

        //Close(m_SocketFD);
    }

void BSDSocket::Poll() {
    /*if (m_SocketFD != NULL) {
        if (m_isServer) {
            pollServer();
        }
        else {
            pollClient();
        }
    }*/
}


#endif
