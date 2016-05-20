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

		virtual ~Entry();

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

		char* m_identifier;

		const char* m_name;///< Pointer to the section of the identifier string where the name starts.
	};

	///@brief Entry used for holding 32-bit integer data.
	class IntEntry : public Entry
	{
	public:

		IntEntry(HostID owner, const char* identifier);

		E_DATA_TYPE GetType() { return EDT_INT; }

		void Set(int32_t integer);

		int32_t Get();

		void Serialize(DataBuffer& out);

		void Deserialize(DataBuffer& in);

	private:

		int32_t m_integer;
	};

	///@brief Entry used for holding 32-bit integer data.
	class LongEntry : public Entry
	{
	public:

		LongEntry(HostID owner, const char* identifier);

		E_DATA_TYPE GetType() { return EDT_LONG; }

		void Set(int64_t integer);

		int64_t Get();

		void Serialize(DataBuffer& out);

		void Deserialize(DataBuffer& in);

	private:

		int64_t m_integer;
	};

	///@brief Entry used for holding 32-bit floating point data.
	class FloatEntry : public Entry
	{
	public:

		FloatEntry(HostID owner, const char* identifier);

		E_DATA_TYPE GetType() { return EDT_FLOAT; }

		void Set(float f);

		float Get();

		void Serialize(DataBuffer& out);

		void Deserialize(DataBuffer& in);

	private:

		float m_float;
	};

	///@brief Entry used for holding 64-bit floating point data.
	class DoubleEntry : public Entry
	{
	public:

		DoubleEntry(HostID owner, const char* identifier);

		E_DATA_TYPE GetType() { return EDT_DOUBLE; }

		void Set(double d);

		double Get();

		void Serialize(DataBuffer& out);

		void Deserialize(DataBuffer& in);

	private:

		double m_double;
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

	///@brief Entry for holding unformatted data (anything the client wishes to send).
	class UnformattedEntry : public Entry
	{
	public:

		UnformattedEntry(HostID owner, const char* identifier);

		~UnformattedEntry();

		E_DATA_TYPE GetType() { return EDT_UNFORMATTED; }

		///@brief Sets the data in the entry.
		void Set(char* data, size_t dataSz);

		///@brief Returns the data in the entry.
		char* Get();

		///@brief Returns the size of the data in the entry.
		size_t GetSize();

		void Serialize(DataBuffer& out);

		void Deserialize(DataBuffer& in);

	private:

		void EnsureCapacity(size_t sz);

		size_t m_capacity;

		char* m_data;
		size_t m_dataSz;
	};
}

#endif