#include <sys/proc_info.h>
#include "BSDSocket.h"

#ifdef RRDTP_PLATFORM_UNIX

using namespace rrdtp;

static int portNumber = 8080;
static int m_SocketFD;


BSDSocket::BSDSocket(DataRecievedCallback dataRecievedCallback, void* userPtr, ConnectionAcceptedCallback connectionAcceptedCallback)
    :Socket(dataRecievedCallback, userPtr, connectionAcceptedCallback)

{

    IsServer();

}
		
BSDSocket::~BSDSocket()
{

}

E_SOCKET_ERROR BSDSocket::Establish(E_SOCKET_PROTOCOL protocol)
{

    if ((m_SocketFD = socket(AF_INET,SOCK_STREAM,protocol)) == -1)
    {
        close(m_SocketFD);
        exit(EXIT_FAILURE);
    }


}


E_SOCKET_ERROR BSDSocket::Connect(const char *ip, unsigned int port, E_SOCKET_PROTOCOL protocol)
{
    bzero(&remoteSocketInfo, sizeof(HostID));

    gethostname(sysHost, MAXHOSTNAME);

    if((hPtr = gethostbyname((const char *) remoteHost)) == NULL)
    {
        perror("System hostname misconfigured." );
        exit(EXIT_FAILURE);
    }

    memcpy((char*)&remoteSocketInfo.sin_addr, hPtr->h_addr, hPtr->h_length);
    remoteSocketInfo.sin_family = AF_INET;
    remoteSocketInfo.sin_port = htons((u_short)portNumber);

    if((connect(m_SocketFD, (struct sockaddr*)&remoteSocketInfo,sizeof(protocol)) < 0))
    {
        close(m_SocketFD);
        exit(EXIT_FAILURE);
    }


}

E_SOCKET_ERROR BSDSocket::Listen(unsigned int port, E_SOCKET_PROTOCOL protocol)
{

    if (bind(m_SocketFD, (struct sockaddr *) &socketInfo, sizeof(struct sockaddr_in) < 0)) {
        close(m_SocketFD);
        perror("bind");
        exit(EXIT_FAILURE);
    }

    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    int socketConnection;

    if ((socketConnection = accept(m_SocketFD, NULL, NULL)) < 0) {
        close(m_SocketFD);
        exit(EXIT_FAILURE);
    }

}

    bool BSDSocket ::IsServer()
    {
        return m_isServer;
    }


   HostID BSDSocket::Accept(E_SOCKET_ERROR *errorCodeOut)
    {
        int newSocket;
        struct sockaddr_in client;
        int c = sizeof(client);

        newSocket = accept(m_SocketFD, (struct sockaddr *)&client, (socklen_t *) &c);

        if (newSocket < 0)
        {
            if (errorCodeOut)
            {
                *errorCodeOut = ESE_FAILURE;
            }
            return -1;
        }

        if(errorCodeOut)
        {
            *errorCodeOut = ESE_SUCCESS;
        }


        return size_t(newSocket);




    }

    HostID BSDSocket::Write(const void *data, size_t sz, HostID hostID)
    {

        if (m_isServer)
        {
            return write(hostID, (const char*)data, sz);
        }

        return write(m_SocketFD, (const char*)data, sz);


    }

HostID BSDSocket :: Read(const void *data, size_t sz, HostID hostID)

{
    int readCount = 0;
    int count = 0;
    int numToRead = 32;



    while(count < numToRead)
    {
        if(readCount == read(m_SocketFD, (void *) data, sz - hostID))
        {
            count += readCount;
            data += readCount;

        }
        else if(readCount < 0)
        {
            close(m_SocketFD);
            exit(EXIT_FAILURE);
        }

        printf("Numbers of bytes read: %d " , count );


    }




}





    void BSDSocket :: Close(int socket)
    {

        //Close(m_SocketFD);
    }

/**void BSDSocket::Poll() {
    if (m_SocketFD != NULL) {
        if (m_isServer) {
            pollServer();
        }
        else {
            pollClient();
        }
    }
}*/






#endif