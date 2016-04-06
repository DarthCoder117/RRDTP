#ifndef Types_H
#define Types_H

namespace rrdtp
{
	///@brief Enum identifying the type of data to be sent in a data packet.
	enum E_DATA_TYPE
	{
		EDT_INT32,
		EDT_INT64,
		EDT_FLOAT,
		EDT_DOUBLE,
		EDT_BOOLEAN,
		EDT_STRING,
		EDT_UNFORMATTED
	};
	
	///@brief Data packet to be sent over the network.
	struct DataPacket
	{
		E_DATA_TYPE m_type;
		void* m_data;
		size_t m_dataSz;
	};
}

#endif