#ifndef Socket_H
#define Socket_H

namespace rrdtp
{	
	///@brief Socket error code enum.
	enum SOCKET_ERROR
	{
		SE_SUCCESS,///< No error.
		SE_FAILURE,///< Generic error.
	};

	///@brief The protocol to use when opening the socket.
	enum SOCKET_PROTOCOL
	{
		TCP,
		UDP
	};
	
	//Callback function pointer types
	typedef void (*ConnectionAcceptedCallback)();
	typedef void (*DataRecievedCallback)(void* data, size_t sz);
	
	///@brief Base class for low level socket communications.
	class Socket
	{		
	public:
		
		Socket(DataRecievedCallback dataRecievedCallback=NULL, void* userPtr=NULL, ConnectionAcceptedCallback connectionAcceptedCallback=NULL)
			:m_userPtr(userPtr),
			m_connectionAcceptedCallback(connectionAcceptedCallback),
			m_dataRecievedCallback(dataRecievedCallback)
		{}
		
		virtual ~Socket(){}
		
		///@brief Opens a socket in client mode and starts listening for incoming packets in a separate thread.
		virtual SOCKET_ERROR connect(const char* ip, unsigned int port, SOCKET_PROTOCOL protocol) = 0;
		
		///@brief Opens socket in server mode and listens for incoming connections and incoming packets.
		///This method is non-blocking and will spawn a separate thread to listen for incoming connections and incoming packets.
		virtual SOCKET_ERROR listen(unsigned int port, SOCKET_PROTOCOL protocol) = 0;
		
		///@brief Close the socket.
		virtual void close() = 0;
		
		///@brief Sends data from this socket.
		///@return The number of bytes actually sent.
		virtual size_t send(const void* data, size_t sz) = 0;
		
	private:
		
		void* m_userPtr;///< User data pointer that will be passed along to callbacks.
		ConnectionAcceptedCallback m_connectionAcceptedCallback;///< Called when a new connection is accepted.
		DataRecievedCallback m_dataRecievedCallback;///< Called when data is recieved from a remote host.
		
		//Copy constructors are private because sockets cannot be copied.
		Socket(const Socket& oth){}
		Socket(Socket& oth){}
	};
}

#endif