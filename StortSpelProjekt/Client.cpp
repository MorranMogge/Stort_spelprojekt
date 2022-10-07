#include "stdafx.h"
#include "Client.h"

void clientFunction(void* param)
{
	ThreadInfo* data = (ThreadInfo*)param;
	std::string temp;
	sf::Packet receivedPacket;
	while (!data->endThread)
	{
		data->socket.receive(receivedPacket); //Receives the packet
		if (receivedPacket >> temp)	//Checks whether or not the packet contains data
		{
			data->receivedstring = temp;
		}
		receivedPacket.clear();
	}
}

Client::Client()
{
	this->ip = sf::IpAddress::getLocalAddress().toString();
	this->port = 2001;
}

Client::Client(std::string ipAddress, int port)
{
	sf::IpAddress ip = sf::IpAddress(ipAddress);

}

Client::~Client()
{
	if (clientThread != nullptr)
	{
		data.endThread = true;
		clientThread->join();
		delete clientThread;
	}
}

void Client::connectToServer(std::string ipAddress, int port)
{
	data.socket.connect(ipAddress, port);
}

void Client::connectToServer()
{
	data.socket.connect(this->ip, this->port);
	data.socket.setBlocking(false);
}

void Client::joinThread()
{
	data.endThread = true;
	clientThread->join();
}

void Client::setIpAndPort(std::string ipAddress, int port)
{
	this->ip = ipAddress;
	this->port = port;
}

bool Client::setupThread()
{
	clientThread = new std::thread(clientFunction, &this->data);
	return clientThread!=nullptr;
}

std::string Client::receive()
{
	return data.receivedstring;
}
