#include "RRDTP.h"
#include "RRDTP/Sockets/WinsockSocket.h"

void dataRecieved(void* data, size_t dataSz)
{
	
}

int main()
{
	//Create server and start listening for connections.
	rrdtp::Socket* serverSocket = new rrdtp::WinsockSocket(&dataRecieved);
	serverSocket.listen("127.0.0.1", 4309, rrdtp::SP_TCP);
	
	//Create client socket and try to connect to the server.
	rrdtp::Socket* clientSocket = new rrdtp::WinsockSocket(&dataRecieved);
	serverSocket.connect("127.0.0.1", 4309, rrdtp::SP_TCP);
}