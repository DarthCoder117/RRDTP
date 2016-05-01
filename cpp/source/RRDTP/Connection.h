/*==============================================================================
The MIT License (MIT)

Copyright (c) 2016 Tanner Mickelson

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

		///@brief Closes the connection.
		void Close();

		///@brief Set a 32-bit integer value to be sent to all subscribed systems (client or server).
		///@param identifier A valid value identifier.
		///@param val The value to set.
		void SetInt(const char* identifier, int32_t val);
		///@brief Get the most recent 32-bit integer value with the specified identifer (client or server).
		///If this is a client, then the client must either own the value, or be subscribed to it for this method to work.
		///@param identifier A valid value identifier.
		///@param defaultVal The default value that will be returned if the identified value was not found.
		///@return The retrieved value.
		int32_t GetInt(const char* identifier, int32_t defaultVal = 0);

		///@brief Set a 64-bit integer value to be sent to all subscribed systems (client or server).
		///@param identifier A valid value identifier.
		///@param val The value to set.
		void SetLong(const char* identifier, int64_t val);
		///@brief Get the most recent 64-bit integer value with the specified identifer (client or server).
		///If this is a client, then the client must either own the value, or be subscribed to it for this method to work.
		///@param identifier A valid value identifier.
		///@param defaultVal The default value that will be returned if the identified value was not found.
		///@return The retrieved value.
		int64_t GetLong(const char* identifier, int64_t defaultVal = 0);

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

		///@brief Gets an entry directly. 
		///@return A pointer to the entry, or NULL if it doesn't exist.
		Entry* GetEntry(const char* identifier);

		///@brief Gets a category.
		///@return A pointer to the category, or NULL if it doesn't exist.
		Category* GetCategory(const char* identifier);

		///@brief Deletes an entry.
		///@param identifier A valid value identifier.
		void Delete(const char* identifier);

		///@brief Called periodically to check for new values and update the local data store with new ones.
		void Poll();

		///@brief Sets the callback that will be used when a value is changed.
		void SetValueChangedCallback(ValueChangedCallback callback) { m_valueChangedCallback = callback; }

	private:

		static void dataRecieved(Socket* self, HostID sender, void* data, size_t dataSz);

		static void clientConnected(Socket* self, HostID client);

		///@brief Sends a value update packet.
		///@param entry The entry to send the packet for.
		///@param target The target host to send the update packet to. Setting this to -1 (the default) will send packets to all connected clients.
		void SendUpdatePacket(Entry* entry, HostID target=-1);

		///@brief Sends a value deletion packet.
		void SendDeletionPacket(const char* identifier);

		///@brief Sends out an update packet for all existing entries.
		void SynchronizeAllEntries(HostID client);
		void SynchronizeAllEntriesImplementation(Category* start, HostID client);

		Socket* m_socket;

		LocalStore m_localDataStore;

		ValueChangedCallback m_valueChangedCallback;
	};
}

#endif