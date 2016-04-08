#ifndef Connection_H
#define Connection_H
#include "RRDTP/Types.h"
#include "RRDTP/Sockets/Socket.h"

namespace rrdtp
{
	///@brief Manages an RRDTP connection.
	class Connection
	{
	public:

		///@brief Starts an RRDTP server connection.
		void StartServer(unsigned int port=4309);

		///@brief Starts an RRDTP client connection.
		void StartClient(const char* ip, unsigned int port = 4309);

	private:

		Socket* m_socket;
	};
}

#endif