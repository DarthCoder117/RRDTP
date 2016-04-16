#ifndef Types_H
#define Types_H

#define RRDTP_VERSION 1

namespace rrdtp
{
	///@brief Enum identifying the type of event a packet represents.
	enum E_EVENT_TYPE
	{
		EET_SUBSCRIBE,///< A client wants to subscribe to a named value.
		EET_UNSUBSCRIBE,///< A client doesn't want to be subscribed to a value anymore.
		EET_VALUE_UPDATE///< The packet contains an updated data value.
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

	///@brief Every RRDTP packet contains a header describing the type of data contained in the packet.
	struct PacketHeader
	{
		unsigned m_protocolVersion : 4;///< First 4 bits is protocol version. This will just be 1 for the initial version of the protocol.

		E_EVENT_TYPE m_eventType : 4;///< The rest of the packet definition depends on the event type. Event type is 4 bits also, because having more than 16 event types is unlikely at this stage.
	};
}

#endif