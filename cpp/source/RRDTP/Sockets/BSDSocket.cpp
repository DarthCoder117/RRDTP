#include "BSDSocket.h"

#ifdef RRDTP_PLATFORM_UNIX

using namespace rrdtp;

BSDSocket::BSDSocket(DataRecievedCallback dataRecievedCallback, void* userPtr, ConnectionAcceptedCallback connectionAcceptedCallback)
	:Socket(dataRecievedCallback, userPtr, connectionAcceptedCallback)
{
	
}
		
BSDSocket::~BSDSocket()
{
	
}

E_SOCKET_ERROR BSDSocket::connect(const char* ip, unsigned int port, E_SOCKET_PROTOCOL protocol)
{
	
}

E_SOCKET_ERROR BSDSocket::listen(unsigned int port, E_SOCKET_PROTOCOL protocol)
{
	
}

void BSDSocket::close()
{
	
}

size_t BSDSocket::send(const void* data, size_t sz)
{
	
}

#endif