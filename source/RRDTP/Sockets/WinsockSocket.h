#ifndef WinsockSocket_H
#define WinsockSocket_H
#include "RRDTP/Platform.h"
#include "RRDTP/Sockets/Socket.h"

#ifdef RRDTP_PLATFORM_WINDOWS

#include<stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock2.h>
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
		
		E_SOCKET_ERROR Accept();
		
		void Close();
		
		size_t Send(const void* data, size_t sz);
		
		void Poll();
		
	private:
		
		///@brief Handles Winsock initialization common between client and server sockets.
		E_SOCKET_ERROR CommonInit();
		
		static WSADATA* m_wsaData;
		SOCKET m_socket;
	};
}

#endif

#endif