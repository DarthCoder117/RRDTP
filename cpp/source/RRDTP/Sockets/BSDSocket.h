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
#ifndef BSDSocket_H
#define BSDSocket_H
#include "RRDTP/Platform.h"
#include "RRDTP/Sockets/Socket.h"

#if defined(RRDTP_PLATFORM_UNIX)
#define MAXHOSTNAME 255


#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <list>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

namespace rrdtp
{
	///@brief Berkely sockets implementation of socket class.
	class BSDSocket : public Socket
	{		
	public:
		
		BSDSocket(DataRecievedCallback dataRecievedCallback=NULL, void* userPtr=NULL, ConnectionAcceptedCallback connectionAcceptedCallback=NULL);
		
		~BSDSocket();

		E_SOCKET_ERROR Connect(const char *ip, unsigned int port, E_SOCKET_PROTOCOL protocol);
		
		E_SOCKET_ERROR Listen(unsigned int port, E_SOCKET_PROTOCOL protocol);

		bool IsServer();

		HostID GetHostID() {return m_socket;}

        HostID Accept(E_SOCKET_ERROR* errorCodeOut=NULL);

		size_t Send(const void* data, size_t sz, HostID host);
		
		void SendAll(const void* data, size_t sz);

        void Close();

	void Poll();


    private:

		E_SOCKET_ERROR CommonInit(E_SOCKET_PROTOCOL protocol);

        void PollServer();
        void PollClient();

        int m_socket;
		bool m_isServer;

        std::list<int> m_connectedClients;

		fd_set m_readFDS;///< FD set for reading.
    };
}

#endif

#endif
