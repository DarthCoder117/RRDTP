#ifndef Types_H
#define Types_H

namespace rrdtp
{
	///@brief Enum identifying the type of data to be sent in a data packet.
	enum DATA_TYPE
	{
		DT_INT32,
		DT_INT64,
		DT_FLOAT,
		DT_DOUBLE,
		DT_BOOLEAN,
		DT_STRING,
		DT_UNFORMATTED
	};
	
	///@brief Data packet to be sent over the network.
	struct DataPacket
	{
		DATA_TYPE m_type;
		void* m_data;
		size_t m_dataSz;
	};
	
	///@brief Error code enum for error checking without using exceptions.
	enum ERROR
	{
		SUCCESS,///< Generic success code
		FAILURE///< Generic error code
	};
}

#endif