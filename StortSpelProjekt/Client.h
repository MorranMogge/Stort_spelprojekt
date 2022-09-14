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
	sf::UdpSocket sendSocket;
	sf::TcpSocket tcpSocket;
	sf::Packet sendPacket;
	sf::Packet receivedPacket;
	
	std::string ip;
	int port;

	std::string tmp;
	std::string id;

	std::thread* clientThread;
public:
	Client();
	Client(std::string ipAddress, int port = 2001);
	~Client();
	void connectToServer(std::string ipAddress, int port);
	void connectToServer();
	void joinThread();
	void setIpAndPort(std::string ipAddress, int port);
	void setIdentifier(std::string id);

	bool setupThread();
	std::string receive();
	void sendToServerUdp(std::string stringToSend);
	void sendToServerUdp();

	void sendToServerTcp();
	void sendToServerTcp(std::string buf);
	void receiveFromServerTcp();

	void saveText(std::string text);
	void saveMsg(std::string text);
	void tempwrite();

	int getport()const;
	std::string getipAdress()const;
};