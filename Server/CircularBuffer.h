#pragma once
#include<iostream>

const unsigned int SIZEOFBUFFER = 256;

class CircularBuffer
{
private:
	char* ringBuffer[SIZEOFBUFFER];
	char* startOfBuffer;
	char* endOfBuffer;
	char* read;
	char* write;
public:
	CircularBuffer();

	void test();
};