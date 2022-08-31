#include "Client.h"

Client::Client(std::string ipAddress, int port)
{
	sf::IpAddress ip = sf::IpAddress(ipAddress);

	socket.connect(ip, 2001);
}

Client::~Client()
{
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
