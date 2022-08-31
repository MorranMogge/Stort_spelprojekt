#pragma once
#include <SFML/Network.hpp>
#include <iostream>

class Client
{
private:
	sf::TcpSocket socket;
	sf::Packet sendPacket;
	sf::Packet receivedPacket;

public:
	Client(std::string ipAddress, int port = 2001);
	~Client();
	std::string receive();

};