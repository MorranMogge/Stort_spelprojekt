#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
{
	this->data = malloc(SIZEOFBUFFER);
	memset(data, 0, SIZEOFBUFFER);
	this->startOfBuffer = (char*)data;
	this->endOfBuffer = (char*)data;
	endOfBuffer += SIZEOFBUFFER;
	this->read = (char*)data;
	this->write = (char*)data;
}

CircularBuffer::~CircularBuffer()
{
	//kanske inte funkar
	memset(data, 0, SIZEOFBUFFER);
	free(data);
}

bool CircularBuffer::addData(const char incomingData[], const std::size_t& recvSize)
{
	if ((write + recvSize) < endOfBuffer) // the write* wont go outside of the circular buffer
	{
		memcpy(write, incomingData, recvSize);
		write += recvSize;
		return true;
	}
	else// reset the write* to be written to the start of the circular buffer
	{
		write = startOfBuffer;
		memcpy(write, incomingData, recvSize);
		write += recvSize;
		return false;
	}

}

bool CircularBuffer::addData(const void*& incomingData, const std::size_t& recvSize)
{
	if ((write + recvSize) < endOfBuffer) // the write* wont go outside of the circular buffer
	{
		memcpy(write, incomingData, recvSize);
		write += recvSize;
		return true;
	}
	else// reset the write* to be written to the start of the circular buffer
	{
		write = startOfBuffer;
		memcpy(write, incomingData, recvSize);
		write += recvSize;
		return false;
	}
}

//returns the packet id 
int CircularBuffer::getPacketId()
{
	int value = -1;
	memcpy(&value, read, sizeof(int));

	return value;
}

void*& CircularBuffer::getData()
{
	void* startOfData;
	startOfData = (void*)this->read;
	return startOfData;
}

void CircularBuffer::advancePointer()
{
	read += sizeof(testPosition);
}

bool CircularBuffer::getIfPacketsLeftToRead()
{
	return write > read;
}
