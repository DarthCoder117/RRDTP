#ifndef Connection_H
#define Connection_H
#include "RRDTP/Types.h"
#include "RRDTP/LocalStore.h"
#include "RRDTP/Sockets/Socket.h"

namespace rrdtp
{
	///@brief Manages an RRDTP connection.
	class Connection
	{
	public:

		Connection();

		~Connection();

		///@brief Starts an RRDTP server connection.
		void StartServer(unsigned int port=4309);

		///@brief Starts an RRDTP client connection.
		void StartClient(const char* ip, unsigned int port = 4309);

		///@brief Set a 32-bit integer value to be sent to all subscribed systems (client or server).
		///@param identifier A valid value identifier.
		///@param val The value to set.
		void SetInt(const char* identifier, int val);
		///@brief Get the most recent 32-bit integer value with the specified identifer (client or server).
		///If this is a client, then the client must either own the value, or be subscribed to it for this method to work.
		///@param identifier A valid value identifier.
		///@param defaultVal The default value that will be returned if the identified value was not found.
		///@return The retrieved value.
		int GetInt(const char* identifier, int defaultVal = 0);

		///@brief Called periodically to check for new values and update the local data store with new ones.
		void Poll();

	private:

		Socket* m_socket;

		LocalStore m_localDataStore;
	};
}

#endif