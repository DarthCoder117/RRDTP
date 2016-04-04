#ifndef WinsockSocket_H
#define WinsockSocket_H
#include "RRDTP/Platform.h"
#include "RRDTP/Sockets/Socket.h"

#ifdef RRDTP_PLATFORM_WINDOWS

#include<stdio.h>
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
		
		SOCKET_ERROR connect(const char* ip, unsigned int port, SOCKET_PROTOCOL protocol);
		
		SOCKET_ERROR listen(unsigned int port, SOCKET_PROTOCOL protocol);
		
		void close() = 0;
		
		size_t send(const void* data, size_t sz);
		
	private:
		
		///@brief Handles Winsock initialization common between client and server sockets.
		SOCKET_ERROR commonInit();
		
		static WSADATA* m_wsaData;
		SOCKET m_socket;
	};
}

#endif

#endif