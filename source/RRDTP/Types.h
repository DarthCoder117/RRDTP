#ifndef Types_H
#define Types_H

#define RRDTP_VERSION 1

namespace rrdtp
{
	///@brief Enum identifying the type of event a packet represents.
	enum E_EVENT_TYPE
	{
		EET_CLIENT_HANDSHAKE,///< Sent to the server once when first connecting.
		EET_SERVER_HANDSHAKE,///< Sent by the server once when a connection is first recieved.
		EET_SET,///< Create a named value on the server or a client.
		EET_DELETE///< Remove a value entry from the server or a client.
	};

	///@brief Enum identifying the type of data to be sent in a data packet.
	enum E_DATA_TYPE
	{
		EDT_INT,///< Signed 32-bit integer
		EDT_LONG,///< Signed 64-bit integer
		EDT_FLOAT,///< single precision floating point
		EDT_DOUBLE,///< double precision floating point
		EDT_BOOLEAN,///< true/false boolean flag
		EDT_STRING,///< string value
		EDT_UNFORMATTED
	};
}

#endif