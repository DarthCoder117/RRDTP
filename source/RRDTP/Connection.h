#ifndef Connection_H
#define Connection_H
#include "RRDTP/Types.h"
#include "RRDTP/LocalStore.h"
#include "RRDTP/Sockets/Socket.h"

namespace rrdtp
{
	class Connection;
	class Entry;

	///@brief Callback for whenever a value is changed.
	typedef void(*ValueChangedCallback)(Connection* connection, Entry* entry);

	///@brief Manages an RRDTP connection.
	class Connection
	{
	public:

		Connection();

		~Connection();

		///@brief Starts an RRDTP server connection.
		void StartServer(unsigned int port=4309);

		///@brief Starts an RRDTP client connection.
		///@return True if the connection attempt was successful. False if the connection failed.
		bool StartClient(const char* ip, unsigned int port = 4309);

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

		///@brief Set a 64-bit integer value to be sent to all subscribed systems (client or server).
		///@param identifier A valid value identifier.
		///@param val The value to set.
		void SetLong(const char* identifier, long val);
		///@brief Get the most recent 64-bit integer value with the specified identifer (client or server).
		///If this is a client, then the client must either own the value, or be subscribed to it for this method to work.
		///@param identifier A valid value identifier.
		///@param defaultVal The default value that will be returned if the identified value was not found.
		///@return The retrieved value.
		long GetLong(const char* identifier, long defaultVal = 0);

		///@brief Set a boolean value to be sent to all subscribed systems (client or server).
		///@param identifier A valid value identifier.
		///@param val The value to set.
		void SetBool(const char* identifier, bool val);
		///@brief Get the most recent boolean value with the specified identifer (client or server).
		///If this is a client, then the client must either own the value, or be subscribed to it for this method to work.
		///@param identifier A valid value identifier.
		///@param defaultVal The default value that will be returned if the identified value was not found.
		///@return The retrieved value.
		bool GetBool(const char* identifier, bool defaultVal = false);

		///@brief Set a string value to be sent to all subscribed systems (client or server).
		///@param identifier A valid value identifier.
		///@param val The value to set.
		void SetString(const char* identifier, const char* str);
		///@brief Get the most recent string value with the specified identifer (client or server).
		///If this is a client, then the client must either own the value, or be subscribed to it for this method to work.
		///@param identifier A valid value identifier.
		///@param defaultVal The default value that will be returned if the identified value was not found.
		///@return A pointer to a copy of the retrieved value. The caller is responsible for ensuring that delete[] is called when finished with the value.
		const char* GetString(const char* identifier, const char* defaultVal = "");

		///@brief Called periodically to check for new values and update the local data store with new ones.
		void Poll();

		///@brief Sets the callback that will be used when a value is changed.
		void SetValueChangedCallback(ValueChangedCallback callback) { m_valueChangedCallback = callback; }

	private:

		static void dataRecieved(Socket* self, HostID sender, void* data, size_t dataSz);

		static void clientConnected(Socket* self, HostID client);

		///@brief Sends a value update packet.
		///@param identifier The value identifier to update.
		///@param entry The entry to send the packet for.
		void SendUpdatePacket(const char* identifier, Entry* entry);

		Socket* m_socket;

		LocalStore m_localDataStore;

		ValueChangedCallback m_valueChangedCallback;
	};
}

#endif