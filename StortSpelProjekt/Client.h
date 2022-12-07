#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

#include "PacketsDataTypes.h"
#include "CircularBufferClient.h"

struct ThreadInfo
{
	sf::TcpSocket socket;
	std::string receivedstring;
	int playerId = -1;
	CircularBufferClient* circularBuffer = nullptr;
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



	bool isConnected;

	sf::SocketSelector selector;
	
	std::string ip;
	int port;

	std::string tmp;
	std::string id;

	std::thread* clientThread;
public:
	Client();
	Client(std::string ipAddress, int port = 2001);
	~Client();

	void setClientId(int nr);

	void connectToServer(std::string ipAddress, int port);
	void connectToServer();
	void joinThread();
	void setIpAndPort(std::string ipAddress, int port);
	void setIdentifier(std::string id);

	bool setupThread();

	void sendToServerTcp();
	void sendToServerTcp(std::string buf);
	void receiveFromServerTcp();

	//returns if a player position has been received
	bool getIfConnected();
	int getport()const;
	int getPlayerId()const;


	CircularBufferClient*& getCircularBuffer();

	std::string getipAdress()const;

	void setPlayerRecv(const bool value);

	template <typename T>
	void sendStuff( T& dataToSend);
};

template <typename T>
inline void Client::sendStuff( T& dataToSend)
{
	//returns the nr of bytes sent
	std::size_t sentSize;
	
	

	if (data.socket.send(&dataToSend, sizeof(dataToSend), sentSize) != sf::Socket::Done)
	{
		//error
		//std::cout << "TCP Couldnt send packet\n";
	}
	else 
	{
		//succeded to send
	}
}