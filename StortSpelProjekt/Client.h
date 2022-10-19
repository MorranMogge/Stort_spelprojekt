#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include "Player.h"

#include "Player.h"
#include "PacketsDataTypes.h"
#include "CircularBufferClient.h"

struct ThreadInfo
{
	sf::TcpSocket socket;
	std::string receivedstring;
	int playerId;
	CircularBufferClient* circularBuffer;
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

	int extractPacketId();
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
	std::string receive();
	void sendToServerUdp(std::string stringToSend);
	void sendToServerUdp();

	void sendToServerTcp();
	void sendToServerTcp(std::string buf);
	void sendToServerTEMPTCP(Player*& currentPlayer);
	void receiveFromServerTcp();

	void saveText(std::string text);
	void saveMsg(std::string text);
	void tempwrite();

	//returns if a player position has been received
	bool getIfConnected();
	int getport()const;
	int getPlayerId()const;

	//returns the players id on the server
	int initTEMPPLAYERS();

	CircularBufferClient*& getCircularBuffer();

	std::string getipAdress()const;

	void setPlayerRecv(const bool value);
};