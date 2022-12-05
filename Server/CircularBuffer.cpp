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
	looped = false;
}

CircularBuffer::~CircularBuffer()
{
	//kanske inte funkar
	memset(startOfBuffer, 0, SIZEOFBUFFER);
	free(data);
}

bool CircularBuffer::addData(const char incomingData[], const std::size_t& recvSize)
{
	if ((write + recvSize) >= endOfBuffer) // the write* wont go outside of the circular buffer
	{
		write = startOfBuffer;
		looped = true;
		memcpy(write, incomingData, recvSize);
		write += recvSize;
		return true;
	}
	else// reset the write* to be written to the start of the circular buffer
	{

		memcpy(write, incomingData, recvSize);
		write += recvSize;
		return false;
	}

}

bool CircularBuffer::addData(const void*& incomingData, const std::size_t& recvSize)
{
	if ((write + recvSize) >= endOfBuffer) // the write* wont go outside of the circular buffer
	{
		write = startOfBuffer;
		looped = true;
		memcpy(write, incomingData, recvSize);
		write += recvSize;
		return true;
	}
	else// reset the write* to be written to the start of the circular buffer
	{

		memcpy(write, incomingData, recvSize);
		write += recvSize;
		return false;
	}
}

//returns the packet id 
int CircularBuffer::peekPacketId()
{
	int value = -1;
	memcpy(&value, read, sizeof(int));

	return value;
}

void* CircularBuffer::getData()
{
	void* startOfData;
	startOfData = (void*)this->read;
	return startOfData;
}

void CircularBuffer::clearBuffer()
{
	memset(startOfBuffer, 0, SIZEOFBUFFER);
	read = startOfBuffer;
	write = startOfBuffer;
}

bool CircularBuffer::getIfPacketsLeftToRead()
{
	//lägg till en check som kollar om buffern är full ex. om den är looped och read är större än write OCH TESTA DEN
	if (looped)
	{
		read = startOfBuffer;
		looped = false;
		return true;
	}
	return write > read;
}

void CircularBuffer::debugWriteMemoryAdress()
{
	std::cout << "write:  " << std::to_string((size_t)write) << " read: " << std::to_string((size_t)read) << std::endl;
}

