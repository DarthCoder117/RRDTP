#ifndef BSDSocket_H
#define BSDSocket_H
#define MAXHOSTNAME 255
#include "RRDTP/Platform.h"
#include "RRDTP/Sockets/Socket.h"

#if defined(RRDTP_PLATFORM_UNIX)
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <list>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>






namespace rrdtp
{
	///@brief Berkely sockets implementation of socket class.
	class BSDSocket : public Socket
	{		
	public:
		
		BSDSocket(DataRecievedCallback dataRecievedCallback=NULL, void* userPtr=NULL, ConnectionAcceptedCallback connectionAcceptedCallback=NULL);
		
		~BSDSocket();

        E_SOCKET_ERROR establish(E_SOCKET_PROTOCOL protocol);

		E_SOCKET_ERROR socket_connect(int ip, struct sockaddr_in* servAddr, int addrLength);
		
		E_SOCKET_ERROR listen(unsigned int port, E_SOCKET_PROTOCOL protocol);

		bool IsServer();

		HostID GetHostID() {return m_socket;}

		HostID Accept(E_SOCKET_ERROR* errorCodeOut = NULL);
		
		static void close(int i);

		size_t Send(const void* data, size_t sz, HostID host=-1);

		void Poll();


    private:

		char sysHost [MAXHOSTNAME+1];
		struct sockaddr_in server;
        struct hostent *hPtr;
        struct sockaddr_in socketAddress;
        struct sockaddr_in remoteSocketInfo;
        struct sockaddr_in socketInfo;
        int ip, m_socket;
        struct hostNamePtr *hostNamePtr1;
        char *remoteHost;
        int portNumber = 8080;
        static int m_SocketFD;



        //std::list<int> m_connectedClients;

        //E_SOCKET_ERROR establish(E_SOCKET_PROTOCOL protocol);




    };
}

#endif

#endif