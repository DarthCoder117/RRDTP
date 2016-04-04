#ifndef BSDSocket_H
#define BSDSocket_H
#include "RRDTP/Platform.h"
#include "RRDTP/Sockets/Socket.h"

#ifdef RRDTP_PLATFORM_UNIX

namespace rrdtp
{
	///@brief Berkely sockets implementation of socket class.
	class BSDSocket : public Socket
	{		
	public:
		
		BSDSocket(DataRecievedCallback dataRecievedCallback=NULL, void* userPtr=NULL, ConnectionAcceptedCallback connectionAcceptedCallback=NULL);
		
		~BSDSocket();
		
		SOCKET_ERROR connect(const char* ip, unsigned int port, SOCKET_PROTOCOL protocol);
		
		SOCKET_ERROR listen(unsigned int port, SOCKET_PROTOCOL protocol);
		
		void close() = 0;
		
		size_t send(const void* data, size_t sz);
		
	private:
		
		
	};
}

#endif

#endif