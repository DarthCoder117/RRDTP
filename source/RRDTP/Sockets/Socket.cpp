#include "RRDTP/Sockets/Socket.h"
#include "RRDTP/Platform.h"

#if defined(RRDTP_PLATFORM_WINDOWS)
#include "RRDTP/Sockets/WinsockSocket.h"
#elif defined(RRDTP_PLATFORM_LINUX)
#include "RRDTP/Sockets/BSDSocket.h"
#endif

using namespace rrdtp;

Socket* Socket::Create(DataRecievedCallback dataRecievedCallback, void* userPtr, ConnectionAcceptedCallback connectionAcceptedCallback)
{
	#if defined(RRDTP_PLATFORM_WINDOWS)
	return new WinsockSocket(dataRecievedCallback, userPtr, connectionAcceptedCallback);
	#elif defined(RRDTP_PLATFORM_LINUX)
	return new BSDSocket(dataRecievedCallback, userPtr, connectionAcceptedCallback);
	#endif
}