#ifndef Connection_H
#define Connection_H
#include "RRDTP/Types.h"

namespace rrdtp
{	
	typedef void (*DataPacketCallback)(const DataPacket&, void*);

	///@brief RRDTP Connection interface. Specific implementations will use different communication methods.
	class Connection
	{
		DataPacketCallback m_packetRecievedCallback;
		void* m_userPtr;
		
	public:
	
		///@brief Constructs an RRDTP connection.
		///@param packetRecievedCallback Callback function for when a packet is recieved.
		///@param userPtr User data pointer to be passed along to the packet reception callback.
		IConnection(DataPacketCallback packetRecievedCallback, void* userPtr);
		
		virtual ~IConnection(){}
		
		///@brief Starts the connection in server mode.
		virtual ERROR startServer(const char* ip = "127.0.0.1", unsigned int port=4309) = 0;
		///@brief Starts the connection in server mode.
		virtual ERROR startClient(const char* ip, unsigned int port=4309) = 0;
		
		///@brief Closes the connection.
		virtual ERROR close() = 0;
		
		///@brief Send a single data packet to the server.
		virtual ERROR sendData(const DataPacket& data) = 0;
		///@brief Send multiple data packets to the server.
		virtual ERROR sendData(const DataPacket& data[], unsigned int count) = 0;
		
		///@brief Request data from the server.
		virtual ERROR requestData(const char* name) = 0;
	};
}

#endif