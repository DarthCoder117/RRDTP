#include "Connection.h"

Connection::Connection(DataPacketCallback packetRecievedCallback, void* userPtr)
	:m_packetRecievedCallback(packetRecievedCallback),
	m_userPtr(userPtr)
{
	
}

