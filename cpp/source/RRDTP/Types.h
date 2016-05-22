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
#ifndef Types_H
#define Types_H
#include <stdint.h>

///@brief The RRDTP protocol version (different from library version)
#define RRDTP_PROTOCOL_VERSION 1

namespace rrdtp
{
	///@brief Enum identifying the type of event a packet represents.
	enum E_EVENT_TYPE
	{
		EET_SET,///< Create a named value on the server or a client.
		EET_DELETE///< Remove a value entry from the server or a client.
	};

	///@brief Enum identifying the type of data to be sent in a data packet.
	enum E_DATA_TYPE
	{
		EDT_INT = 0,///< Signed 32-bit integer
		EDT_LONG = 1,///< Signed 64-bit integer
		EDT_FLOAT = 2,///< single precision floating point
		EDT_DOUBLE = 3,///< double precision floating point
		EDT_BOOLEAN = 4,///< true/false boolean flag
		EDT_STRING = 5,///< string value
		EDT_UNFORMATTED = 6
	};
}

#endif
