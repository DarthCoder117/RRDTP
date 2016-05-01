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
#ifndef DataBuffer_H
#define DataBuffer_H
#include <stdio.h>
#include <string.h>

namespace rrdtp
{
	///@brief Buffer for storing and manipulating packet data easily.
	class DataBuffer
	{
	public:

		DataBuffer(unsigned char* buffer, size_t bufferSz)
			:m_size(bufferSz),
			m_buffer(buffer),
			m_position((size_t)buffer)//Start reading/writing from beginning.
		{}

		///@brief Write built-in data types to the buffer.
		template <typename T>
		bool Write(const T& val)
		{
			if (!CheckCapacity(sizeof(T)))
			{
				return false;
			}

			*((T*)m_position) = val;
			m_position += sizeof(T);

			return true;
		}

		///@brief Write data to the buffer.
		bool Write(const unsigned char* data, size_t sz)
		{
			if (!CheckCapacity(sz))
			{
				return false;
			}

			memcpy((void*)m_position, (void*)data, sz);
			m_position += sz;

			return true;
		}

		///@brief Reads built-in data types from the buffer.
		template <typename T>
		bool Read(T& outVal)
		{
			if (!CheckCapacity(sizeof(T)))
			{
				return false;
			}

			outVal = *((T*)m_position);
			m_position += sizeof(T);

			return true;
		}

		///@brief Read data from the buffer.
		bool Read(const unsigned char* dataOut, size_t sz)
		{
			if (!CheckCapacity(sz))
			{
				return false;
			}

			memcpy((void*)dataOut, (void*)m_position, sz);
			m_position += sz;

			return true;
		}

		///@bruef Gets the size of the buffer.
		size_t GetBufferSize()
		{
			return m_size;
		}

		///@brief Gets the current position in the buffer.
		///This can be useful for calculating the actual size of the buffer when writing to a buffer that's larger than needed.
		size_t GetPosition()
		{
			return m_position-(size_t)m_buffer;
		}

	private:

		///@brief Checks if the buffer has enough space left to read/write sz bytes of data.
		bool CheckCapacity(size_t sz)
		{
			return (m_position + sz) - (size_t)m_buffer <= m_size;
		}

		size_t m_size;

		unsigned char* m_buffer;
		size_t m_position;
	};
}

#endif