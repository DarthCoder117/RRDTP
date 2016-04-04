#ifndef Session_H
#define Session_H

namespace rrdtp
{
	///@brief A Session caches and tracks changes in data and manages the connection.
	class Session
	{
		asio::io_service m_ioService;
		
	public:
		
		///@brief Constructs a session with an existing connection.
		Session(IConnection* connection);
		
		///@brief Send a 32 bit integer.
		void sendInt32(const char* name, int val);
		///@brief Send a 64 bit integer.
		void sendInt64(const char* name, long val);
		
		///@brief Send a float value.
		void sendFloat(const char* name, float val);
	};
}

#endif