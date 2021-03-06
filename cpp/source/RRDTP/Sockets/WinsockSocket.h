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
#ifndef WinsockSocket_H
#define WinsockSocket_H
#include "RRDTP/Platform.h"
#include "RRDTP/Sockets/Socket.h"

#ifdef RRDTP_PLATFORM_WINDOWS
#include <list>
#include<stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock2.h>
#include <windows.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

namespace rrdtp
{
	///@brief Win32 implementation of socket class.
	class WinsockSocket : public Socket
	{		
	public:
		
		WinsockSocket(DataRecievedCallback dataRecievedCallback=NULL, void* userPtr=NULL, ConnectionAcceptedCallback connectionAcceptedCallback=NULL);
		
		~WinsockSocket();
		
		E_SOCKET_ERROR Connect(const char* ip, unsigned int port, E_SOCKET_PROTOCOL protocol);
		
		E_SOCKET_ERROR Listen(unsigned int port, E_SOCKET_PROTOCOL protocol);
		
		bool IsServer();

		HostID GetHostID() { return m_socket; }

		HostID Accept(E_SOCKET_ERROR* errorCodeOut = NULL);
		
		void Close();
		
		size_t Send(const void* data, size_t sz, HostID host=-1);

		void SendAll(const void* data, size_t sz);
		
		void Poll();
		
	private:
		
		void PollServer();
		void PollClient();

		///@brief Handles Winsock initialization common between client and server sockets.
		E_SOCKET_ERROR CommonInit(E_SOCKET_PROTOCOL protocol);
		
		bool m_isServer;

		static WSADATA* m_wsaData;
		SOCKET m_socket;

		std::list<SOCKET> m_connectedClients;///< List of connected clients (only used on server sockets).

		fd_set m_readFDS;///< FD set for reading.
	};
}

#endif

#endif