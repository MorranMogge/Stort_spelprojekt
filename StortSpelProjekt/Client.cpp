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
	this->ip = ipAddress;
	this->tmp = "empty";
	this->port = 2001;
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

void Client::setIdentifier(std::string id)
{

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

//used for sending string
void Client::sendToServer(std::string stringToSend)
{
	sendPacket << stringToSend;

	
	if (sendSocket.send(sendPacket, this->ip, port) == sf::Socket::Done)
	{
		//sent
		std::cout << "sent data to server\n";
	}
	else
	{
		std::cout << "failed to send data to server\n";
	}
	sendPacket.clear();
}

//used for sending already saved MSG
void Client::sendToServer()
{
	sendPacket << this->tmp;


	if (sendSocket.send(sendPacket, this->ip, port) == sf::Socket::Done)
	{
		//sent
		std::cout << "sent data to server\n";
	}
	else
	{
		std::cout << "failed to send data to server\n";
	}
	sendPacket.clear();
}


void Client::saveText(std::string text)
{
	id = text;
}

void Client::saveMsg(std::string text)
{
	this->tmp = text;
}

void Client::tempwrite()
{
	std::cout << id << std::endl;
}

int Client::getport() const
{
	return this->port;
}

std::string Client::getipAdress() const
{
	return this->ip;
}