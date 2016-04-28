#ifndef Entry_H
#define Entry_H
#include "RRDTP/Types.h"
#include "RRDTP/DataBuffer.h"
#include "RRDTP/Sockets/Socket.h"
#include <string>

namespace rrdtp
{
	///@brief Base entry class for storing values in the local store.
	class Entry
	{
	public:

		///@brief Create an Entry of the specified type.
		static Entry* Create(const char* identifier, E_DATA_TYPE type);

		Entry(HostID owner, const char* identifier);

		virtual ~Entry() {}

		HostID GetOwner();

		const char* GetName();

		const char* GetIdentifier();

		virtual E_DATA_TYPE GetType() = 0;

		///@brief Prepare the data for sending over the network.
		///@param out The DataBuffer to write data into.
		virtual void Serialize(DataBuffer& out) = 0;

		///@brief Set the value of the entry directly from the data recieved over network.
		///@param in The DataBuffer to read data from.
		virtual void Deserialize(DataBuffer& in) = 0;

	protected:

		Entry(const Entry& oth) {}

		HostID m_owner;

		const char* m_identifier;

		const char* m_name;///< Pointer to the section of the identifier string where the name starts.
	};

	///@brief Entry used for holding 32-bit integer data.
	class IntEntry : public Entry
	{
	public:

		IntEntry(HostID owner, const char* identifier);

		E_DATA_TYPE GetType() { return EDT_INT; }

		void Set(int integer);

		int Get();

		void Serialize(DataBuffer& out);

		void Deserialize(DataBuffer& in);

	private:

		int m_integer;
	};

	///@brief Entry used for holding 32-bit integer data.
	class LongEntry : public Entry
	{
	public:

		LongEntry(HostID owner, const char* identifier);

		E_DATA_TYPE GetType() { return EDT_LONG; }

		void Set(long integer);

		long Get();

		void Serialize(DataBuffer& out);

		void Deserialize(DataBuffer& in);

	private:

		long m_integer;
	};

	///@brief Entry used for holding boolean data.
	class BooleanEntry : public Entry
	{
	public:

		BooleanEntry(HostID owner, const char* identifier);

		E_DATA_TYPE GetType() { return EDT_BOOLEAN; }

		void Set(bool b);

		bool Get();

		void Serialize(DataBuffer& out);

		void Deserialize(DataBuffer& in);

	private:

		bool m_boolean;
	};

	///@brief Entry for holding a string.
	class StringEntry : public Entry
	{
	public:

		StringEntry(HostID owner, const char* identifier);

		~StringEntry();

		E_DATA_TYPE GetType() { return EDT_STRING; }

		///@brief Sets the string value in the entry.
		void Set(const char* str);

		///@brief Returns the string value in the entry.
		const char* Get();

		void Serialize(DataBuffer& out);

		void Deserialize(DataBuffer& in);

	private:

		void EnsureCapacity(size_t sz);

		size_t m_capacity;

		char* m_string;
	};
}

#endif