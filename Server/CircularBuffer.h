#pragma once
#include<iostream>
#include<string>
#include"PacketsDataTypes.h"

static const unsigned int SIZEOFBUFFER = 200000;

class CircularBuffer
{
private:
	void* data;
	char* ringBuffer[SIZEOFBUFFER];
	char* startOfBuffer;
	char* endOfBuffer;
	char* read;
	char* write;
	bool looped;
public:
	CircularBuffer();
	~CircularBuffer();

	//kopierar datan från inkommande char* []
	//returnerar false om det inte finns någon plats kvar FALSE FUNKAR INTE NU
	bool addData(const char incomingData[], const std::size_t& recvSize);

	//kopierar datan från inkommande void*
	//returnerar false om det inte finns någon plats kvar FALSE FUNKAR INTE NU
	bool addData(const void*& incomingData, const std::size_t& recvSize);

	//returns an int enum that is the identifier for the packet type
	int peekPacketId();//peek

	//returns a char* that points to the packet
	void* getData();

	//returns if there are packets left to read 
	bool getIfPacketsLeftToRead();

	template <typename T>
	T* readData(int nr = 1);

	void debugWriteMemoryAdress();
};

template<typename T>
inline T* CircularBuffer::readData(int nr)
{
	if ((read + sizeof(T)) >= endOfBuffer)
	{
		read = startOfBuffer;
	}
	T* ptr = (T*)read;
	read += sizeof(T) * nr;
	return ptr;
}