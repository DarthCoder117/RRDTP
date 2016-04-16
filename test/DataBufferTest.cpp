#include <RRDTP/DataBuffer.h>
#include <cassert>

struct TestStruct
{
	TestStruct(bool a=false, bool b=false, bool c=false, int x=0, int y=0, int z=0)
		:A(a), B(b), C(c), X(x), Y(y), Z(z)
	{}

	bool A, B, C;

	int X, Y, Z;
};

int main()
{
	unsigned char* data = new unsigned char[1024];
	size_t dataSz = 1024;
	memset(data, 0, dataSz);

	//Test writing data
	rrdtp::DataBuffer writeBuffer(data, dataSz);

	assert(writeBuffer.Write<int>(12345));
	assert(writeBuffer.Write<long>(987654321));

	TestStruct testWriteStruct(true, false, true, 1, 2, 3);
	assert(writeBuffer.Write((const unsigned char*)&testWriteStruct, sizeof(TestStruct)));

	//Test reading data
	rrdtp::DataBuffer readBuffer(data, dataSz);

	int testInt = 0;
	assert(readBuffer.Read<int>(testInt));
	assert(testInt == 12345);

	long testLong = 0;
	assert(readBuffer.Read<long>(testLong));
	assert(testLong == 987654321);

	TestStruct testReadStruct;
	assert(writeBuffer.Read((const unsigned char*)&testReadStruct, sizeof(TestStruct)));
	assert(memcmp(&testWriteStruct, &testReadStruct, sizeof(TestStruct)));

	return 0;
}