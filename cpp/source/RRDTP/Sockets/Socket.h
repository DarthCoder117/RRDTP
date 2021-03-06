/*==============================================================================
The MIT License (MIT)

Copyright (c) 2016 Tanner Mickelson & The RRDTP Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
==============================================================================*/
#ifndef Socket_H
#define Socket_H
#include <cstdio>

namespace rrdtp
{	
	///@brief Type for identifying a unique remote host.
	typedef size_t HostID;

	///@brief The host ID for the local system.
	const HostID LOCAL_HOST_ID = -1;

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
	
	class Socket;

	///@brief Callback for when a new client connects (server sockets only).
	///@param self The socket the event occured for.
	///@param client The host ID of the client that connected to the server.
	typedef void (*ConnectionAcceptedCallback)(Socket* self, HostID client);
	///@brief Callback for when data is recieved on the socket.
	///@param self The socket the event occured for.
	///@param sender The host ID of the socket that sent the message (server sockets only).
	///@param data Pointer to the data recieved.
	///@param sz The amount of data recieved (in bytes).
	typedef void (*DataRecievedCallback)(Socket* self, HostID sender, void* data, size_t sz);
	
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
		
		///@brief Check whether this socket was opened in server mode or not.
		virtual bool IsServer() = 0;

		///@return The user data pointer set when the socket was created.
		void* GetUserData() { return m_userPtr; }

		///@brief Accepts the next incoming connection in the queue.
		///@param errorCodeOut Optional error code out parameter.
		///@return Returns an ID to refer to the accepted connection, or returns -1 if an error occurred.
		///@deprecated This will be handled by the server socket in the Poll method...
		virtual HostID Accept(E_SOCKET_ERROR* errorCodeOut=NULL) = 0;
		
		///@brief Close the socket.
		virtual void Close() = 0;

		///@brief Sends data from this socket.
		///@param host The remote host to send the data to. This is only needed for server sockets and is ignored for clients.
		///@return The number of bytes actually sent.
		virtual size_t Send(const void* data, size_t sz, HostID host=-1) = 0;
		
		///@brief Sends data to all connected clients (or just sends to the server if this is a client socket).
		virtual void SendAll(const void* data, size_t sz) = 0;

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
