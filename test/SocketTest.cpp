#include "RRDTP.h"
#include "RRDTP/Sockets/WinsockSocket.h"

void serverDataRecieved(void* data, size_t dataSz)
{

}

void clientDataRecieved(void* data, size_t dataSz)
{
	
}

int main()
{
	//Create server and start listening for connections.
	rrdtp::Socket* serverSocket = rrdtp::Socket::Create(&serverDataRecieved);
	serverSocket->Listen(4309, rrdtp::ESP_TCP);
	
	//Create client socket and try to connect to the server.
	rrdtp::Socket* clientSocket = rrdtp::Socket::Create(&clientDataRecieved);
	serverSocket->Connect("127.0.0.1", 4309, rrdtp::ESP_TCP);
}