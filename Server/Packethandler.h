#pragma once
#include <iostream>

//extracts the data thats been received
void allocateDataForPackets(const char data[], void*& dataPointer, const std::size_t& recvSize)
{
	dataPointer = malloc(recvSize);
	memcpy(dataPointer, data, recvSize);
}

