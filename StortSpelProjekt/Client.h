#pragma once
#include <SFML/Network.hpp>
#include <iostream>

class Client
{
private:
	sf::TcpSocket socket;
	sf::Packet sendPacket;
	sf::Packet receivedPacket;

	std::string ip;
	int port;
public:
	Client();
	Client(std::string ipAddress, int port = 2001);
	~Client();
	void connectToServer(std::string ipAddress, int port);
	void connectToServer();
	void setIpAndPort(std::string ipAddress, int port);
	std::string receive();

};