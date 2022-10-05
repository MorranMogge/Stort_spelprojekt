#pragma once
#include <iostream>

//extracts the data thats been received
void* extractData(const char data[], const std::size_t& recvSize)
{
	void* d = malloc(recvSize);
	memcpy(d, data, recvSize);
	return d;
}