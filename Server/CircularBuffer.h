#pragma once
#include<iostream>
#include<string>
#include"PacketsDataTypes.h"

static const unsigned int SIZEOFBUFFER = 400;

class CircularBuffer
{
private:
	void* data;
	char* ringBuffer[SIZEOFBUFFER];
	char* startOfBuffer;
	char* endOfBuffer;
	char* read;
	char* write;
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
	int getPacketId();//peek

	//returns a char* that points to the packet
	void* getData();

	//advance the read pointer for data that has been read
	//void advancePointer();

	//returns if there are packets left to read 
	bool getIfPacketsLeftToRead();

	template <typename T>
	T* readData(int nr = 1);

	void debugWriteMemoryAdress();

	template<typename T>
	void advancePointer(const T& type);
};

template<typename T>
inline T* CircularBuffer::readData(int nr)
{
	T* ptr = (T*)read;
	read += sizeof(T) * nr;
	return ptr;
}

//advance the read pointer with the data type that has been read
//first getPacketId() to identify the struct the data is then T = packetStruct
template<typename T>
inline void CircularBuffer::advancePointer(const T& type)
{
	read += sizeof(T);
}