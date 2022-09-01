#include "Client.h"

void clientFunction(void* param)
{
	ThreadInfo* data = (ThreadInfo*)param;
	int counter = 0;
	std::string temp;
	sf::Packet receivedPacket;
	while (!data->endThread)
	{
		data->socket.receive(receivedPacket);
		if (receivedPacket >> temp)
		{
			data->receivedstring = temp;
		}
		receivedPacket.clear();
	}
}

Client::Client()
{
}

Client::Client(std::string ipAddress, int port)
{
	sf::IpAddress ip = sf::IpAddress(ipAddress);

}

Client::~Client()
{
	data.endThread = true;
	clientThread->join();
	if (clientThread != nullptr) delete clientThread;
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
