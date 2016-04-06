#ifndef Socket_H
#define Socket_H
#include <cstdio>

namespace rrdtp
{	
	///@brief Type for identifying a unique remote host.
	typedef size_t HostID;

	///@brief Socket error code enum.
	enum E_SOCKET_ERROR
	{
		ESE_SUCCESS,///< No error.
		ESE_FAILURE,///< Generic error.
	};

	///@brief The protocol to use when opening the socket.
	enum E_SOCKET_PROTOCOL
	{
		ESP_TCP,
		ESP_UDP
	};
	
	//Callback function pointer types
	typedef void (*ConnectionAcceptedCallback)();
	typedef void (*DataRecievedCallback)(void* data, size_t sz);
	
	///@brief Base class for low level socket communications.
	class Socket
	{		
	public:
		
		///@brief Constructs a Socket object. This is for internal use only. Use Socket::Create instead to create the correct kind of socket for your configuration.
		Socket(DataRecievedCallback dataRecievedCallback, void* userPtr, ConnectionAcceptedCallback connectionAcceptedCallback)
			:m_userPtr(userPtr),
			m_connectionAcceptedCallback(connectionAcceptedCallback),
			m_dataRecievedCallback(dataRecievedCallback)
		{}

		///@brief Creates a socket object of the correct type for the current configuration.
		static Socket* Create(DataRecievedCallback dataRecievedCallback = NULL, void* userPtr = NULL, ConnectionAcceptedCallback connectionAcceptedCallback = NULL);
		
		virtual ~Socket(){}
		
		///@brief Opens a socket in client mode and starts listening for incoming packets in a separate thread.
		virtual E_SOCKET_ERROR Connect(const char* ip, unsigned int port, E_SOCKET_PROTOCOL protocol) = 0;
		
		///@brief Opens socket in server mode and listens for incoming connections and incoming packets.
		virtual E_SOCKET_ERROR Listen(unsigned int port, E_SOCKET_PROTOCOL protocol) = 0;
		
		///@brief Accepts the next incoming connection in the queue.
		virtual E_SOCKET_ERROR Accept() = 0;
		
		///@brief Close the socket.
		virtual void Close() = 0;
		
		///@brief Sends data from this socket.
		///@return The number of bytes actually sent.
		virtual size_t Send(const void* data, size_t sz) = 0;
		
		///@brief Polls the socket for incoming packets.
		///This is where data should be recieved from the socket, and callbacks should be triggered.
		virtual void Poll() = 0;
		
	protected:

		void* m_userPtr;///< User data pointer that will be passed along to callbacks.
		ConnectionAcceptedCallback m_connectionAcceptedCallback;///< Called when a new connection is accepted.
		DataRecievedCallback m_dataRecievedCallback;///< Called when data is recieved from a remote host.
		
	private:

		//Copy constructors are private because sockets cannot be copied.
		Socket(const Socket& oth){}
	};
}

#endif