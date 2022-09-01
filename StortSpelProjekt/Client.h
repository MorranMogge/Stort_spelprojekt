#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

struct ThreadInfo
{
	sf::TcpSocket socket;
	std::string receivedstring;
	bool endThread = false;
};

class Client
{
private:
	ThreadInfo data;
	sf::Packet sendPacket;
	sf::Packet receivedPacket;
	
	std::string ip;
	int port;

	std::string tmp;

	std::thread* clientThread;
public:
	Client();
	Client(std::string ipAddress, int port = 2001);
	~Client();
	void connectToServer(std::string ipAddress, int port);
	void connectToServer();
	void joinThread();
	void setIpAndPort(std::string ipAddress, int port);
	bool setupThread();
	std::string receive();

};