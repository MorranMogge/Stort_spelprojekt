#include "Client.h"

Client::Client()
{
}

Client::Client(std::string ipAddress, int port)
{
	sf::IpAddress ip = sf::IpAddress(ipAddress);

}

Client::~Client()
{
}

void Client::connectToServer(std::string ipAddress, int port)
{
	socket.connect(ipAddress, port);
}

void Client::connectToServer()
{
	socket.connect(this->ip, this->port);
}

void Client::setIpAndPort(std::string ipAddress, int port)
{
	this->ip = ipAddress;
	this->port = port;
}

std::string Client::receive()
{
	receivedPacket.clear();
	std::string receivedstring;
	socket.receive(receivedPacket);
	if (receivedPacket >> receivedstring)
	{
		// Data extracted successfully...
		return receivedstring;
	}
	return "";
}
