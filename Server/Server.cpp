#include <iostream>
#include <string>
#include "../StortSpelProjekt/PackageTypes.h"

#include <SFML/Network.hpp>
#include <chrono>
#include <thread>
#include <vector>
#include "player.h"
#include "PacketsDataTypes.h"
#include "Packethandler.h"
#include "CircularBuffer.h"

const short MAXNUMBEROFPLAYERS = 2;

struct acceptMePacketData
{
	std::string identifier;
	std::string s;
};

struct wow
{
	unsigned short packetId;
	unsigned short playerId;
	float xPos;
	float yPos;
	float zPos;
};

struct userData
{
	sf::IpAddress ipAdress;
	std::string userName;
	sf::TcpSocket tcpSocket;
	int playerId;

	player playa;
};

struct threadInfo
{
	userData* user;
	float pos[3];
	bool ifUserRecv;
	CircularBuffer* circBuffer;
};

struct serverData
{
	bool endServer = false;
	sf::UdpSocket socket;
	sf::UdpSocket sendSocket;
	sf::TcpListener tcpListener;
	sf::TcpSocket tcpSocket;
	userData users[MAXNUMBEROFPLAYERS];
	unsigned short port;
	sf::Packet packet;
};

//example
/*
struct MyStruct
{
	float       number;
	sf::Int8    integer;
	std::string str;
};


sf::Packet& operator <<(sf::Packet& packet, const MyStruct& m)
{
	return packet << m.number << m.integer << m.str;
}

sf::Packet& operator >>(sf::Packet& packet, MyStruct& m)
{
	return packet >> m.number >> m.integer >> m.str;
}
*/
//void handleReceivedData(void* param)
//{
//	serverData* data = (serverData*)param;
//	sf::Packet packet;
//	sf::IpAddress remoteAddress;
//
//	while (!data->endServer)
//	{
//		if (data->socket.receive(packet, data->users[0].ipAdress, data->port))
//		{
//			for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
//			{
//				if (data->users[i].ipAdress == remoteAddress)
//				{
//					std::string receivedString;
//					std::cout << "received data from address: " << remoteAddress.toString() << std::endl;
//
//					packet >> receivedString;
//					std::cout << "Received string from client: " << receivedString << std::endl;
//
//					data->packet = packet;
//					packet.clear();
//				}
//			}
//		}
//	}
//
//};

bool receiveDataUdp(sf::Packet& receivedPacket, serverData &data, unsigned short& packetIdentifier)
{
	//remote adress
	sf::IpAddress remoteAddress;
	

	if (data.socket.receive(receivedPacket, remoteAddress, data.port) == sf::Socket::Done)
	{
		std::string receivedString;
		std::cout << "UDP received data from address: " << remoteAddress.toString() << std::endl;

		return true;
	}
	else
	{
		std::cout << "UDP failure\n";
	}
};


void sendDataUdp(sf::Packet& sentPacket, serverData& data, std::string remoteIpAdress, unsigned short remotePort)
{
	if (data.sendSocket.send(sentPacket, remoteIpAdress, remotePort) == sf::Socket::Done)
	{
		//sent
		std::cout << "UDP sent data to server\n";
	}
	else
	{
		std::cout << "UDP failed to send data to server\n";
	}
};

bool receiveDataTcp(sf::Packet& receivedPacket, serverData& data, const int playerid)
{
	if (data.users[playerid].tcpSocket.receive(receivedPacket) != sf::Socket::Done)
	{
		//std::cout << "failed to receive TCP\n";
		return false;
	}
	else
	{
		std::string receivedString;
		std::cout << "TCP received data from address: " << data.tcpSocket.getRemoteAddress().toString() << std::endl;

		return true;
	}
};

void setupTcp(serverData& data)
{
	unsigned short listenport = 2001;
	data.tcpListener.listen(listenport);
	std::cout << "TCP listening on port: " << std::to_string((int)listenport) << std::endl;
};

void acceptPlayers(serverData& data)
{
	std::cout << "Accepting players TCP socket\n";

	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		if (data.tcpListener.accept(data.users[i].tcpSocket) == sf::TcpListener::Done)
		{
			std::cout << "TCP Accepted new player ipAdress: " << data.users[i].tcpSocket.getRemoteAddress().toString() << std::endl;
			data.users[i].ipAdress = data.users[i].tcpSocket.getRemoteAddress().toString();
			data.users[i].userName = "fixa username " + std::to_string(i + 1);
			data.users[i].playerId = i;
		}
		
	}
};

void sendDataAllPlayers(testPosition &posData, serverData& serverData)
{
	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		std::size_t recvSize;
		if (serverData.users[i].tcpSocket.send(&posData, sizeof(testPosition), recvSize) != sf::Socket::Done)
		{
			//error
			//std::cout << "Couldnt send data to player from array slot: " << std::to_string(i) << std::endl;
		}
		else
		{
			//std::cout << "sent data to player: " << data.users[i].tcpSocket.getRemoteAddress().toString() << std::endl;
		}
	}
};

void sendPositionDataToPlayers(sf::Packet& packet, serverData& data)
{
	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{

		if (data.users[i].tcpSocket.send(packet) != sf::Socket::Done)
		{
			//error
			//std::cout << "Couldnt send data to player from array slot: " << std::to_string(i) << std::endl;
		}
		else
		{

			std::cout << "sent data to player: " << data.users[i].tcpSocket.getRemoteAddress().toString() << std::endl;
		}
	}
};

void packetId(sf::Packet& packet)
{
	//sendPacket >> id >> n >> s;
	unsigned short packetIdentifier;
	int nr;
	std::string s;

	packet >> packetIdentifier;
	std::cout << "packetId: " << std::to_string(packetIdentifier) << std::endl;
	
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	int playerid = -1;
	unsigned short packId = 3;


	switch (packetIdentifier)
	{
	case 1:
		packet >> s;
		std::cout << "Packet identifier: " << packetIdentifier << ", received string: " << s << std::endl;
	
		break;
	case 2:
		packet >> nr >> s;
		std::cout << "packet ID: " << packetIdentifier << ", Nr: " << nr << ", string: " << s << std::endl;
		break;

	case 3:
		//sending position data to all players
		packet << packId << playerid << x << y << z;
		//TA EMOT POSITION
		break;
	}
};

int extractPacketId(sf::Packet& packet)
{
	unsigned short packetIdentifier;
	
	packet >> packetIdentifier;
	//std::cout << "packetId: " << std::to_string(packetIdentifier) << std::endl;
	return packetIdentifier;
};

int extractBinaryPacketId(char* pointerToData[])
{
	int idPacket;
	memcpy(&idPacket, pointerToData, sizeof(int));
	
	return idPacket;
}

//test för att flytta pekaren
float exfloatTest(char* pointerToData[], const std::size_t& recvSize)
{
	float position = -1.0;
	//to move the pointer
	int bytesToMove = sizeof(int) + sizeof(int) + sizeof(float);
	char* test = pointerToData[0];
	char init = ' ';
	//test =  test +  bytesToMove;
	test++;
	test++;
	test++;
	test++;
	
	//void* vp = malloc(recvSize);
	//memcpy(vp, pointerToData, recvSize);
	//vp = (char*)vp + bytesToMove;

	memcpy(&position, test, sizeof(int));
	
	//radera minnet 

	return position;
}

void recvData(void* param, userData* user)//thread to recv data
{
	threadInfo* data = (threadInfo*)param;
	sf::Packet packet;

	std::cout << "ip from socket in thread: " << user->tcpSocket.getRemoteAddress().toString() << std::endl;
	while (1)
	{
		//std::size_t recvSize;
		std::size_t recv;
		char datapointer[256];

		if (user->tcpSocket.receive(&datapointer, 256, recv) != sf::Socket::Done)
		{
			//error
		}
		else
		{
			
			data->circBuffer->addData(datapointer, recv);
			
			//std::cout << "size_t: " << std::to_string(sizeof(testPosition)) << std::endl;
			//std::cout << "testing exfloatTest(): " << std::to_string(testc) << std::endl;
			
			//data->user->playa.setPosition(wow->x, wow->y, wow->z);

			//std::string packetsLeft = "false";
			//if (data->circBuffer->getIfPacketsLeftToRead())packetsLeft = "true";



			//std::cout << "check if circbuffer have packets = " << packetsLeft << std::endl;
			//testPosition* tst = data->circBuffer->readData<testPosition>();

			//std::cout << "Checking return value from circular buffer testPosition.x: " << std::to_string(tst->x) << std::endl;
			//std::cout << "Memory adress: " << circBuffer.getData() << std::endl;

			//data->pos[0] = tst->x;
			//data->pos[1] = tst->y;
			//data->pos[2] = tst->z;
		}
		
		//std::cout << "userName: " << data->user->userName << std::endl;
	}
};

void sendIdToAllPlayers(serverData& data)
{
	int packetid = 10;

	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		idProtocol protocol;
		protocol.packetId = 4;
		protocol.assignedPlayerId = data.users[i].playerId;


		std::size_t sentSize;

		if (data.users[i].tcpSocket.send(&protocol, sizeof(protocol), sentSize) != sf::Socket::Done)
		{
			//error
			std::cout << "sendIdToAllPlayers(), couldnt send id to player: " << std::to_string(i) << std::endl;
		}
		else
		{
			std::cout << "sendIdToAllPlayers() sent to player nr: " << std::to_string(data.users[i].playerId) << std::endl;
		}
	}
};

int main()
{
	std::string identifier;
	std::string s = "empty";
	// Group the variables to send into a packet
	sf::Packet packet;
	sf::Packet receivedPacket;

	float frequency = 30.f;


	std::cout << std::to_string(sizeof(float)) << std::endl;

	std::vector<player> players;

	//sf::UdpSocket socket;
	std::string connectionType, mode;

	serverData data;
	sf::IpAddress remoteAddress;

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();

	data.port = 2001;

	std::cout << ip.toString() << "\n" << "port: " << data.port << std::endl;
	//serverData* serverData;
	//std::cout << "Starting handleReceiveData thread!\n";
	//std::thread* serverThread = new std::thread(handleReceivedData, &serverData);

	if (data.socket.bind(data.port) != sf::Socket::Done)
	{
		std::cout << "UDP Failed to bind UDP servern\n";
		return -1;
	}
	else
	{
		std::cout << "UDP Successfully bound socket\n";
	}
	
	std::chrono::time_point<std::chrono::system_clock> start;
	start = std::chrono::system_clock::now();
	float timerLength = 1.2f;


	setupTcp(data);
	
	acceptPlayers(data);

	sendIdToAllPlayers(data);

	CircularBuffer* circBuffer = new CircularBuffer();
	std::thread* recvThread[MAXNUMBEROFPLAYERS];
	threadInfo threadData[MAXNUMBEROFPLAYERS];
	//threadData.ifUserRecv = false;
	
	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		threadData[i].pos[0] = 22.0f;
		threadData[i].pos[1] = 12.0f;
		threadData[i].pos[2] = -22.0f;
		threadData[i].circBuffer = circBuffer;
		recvThread[i] = new std::thread(recvData, &threadData[i], &data.users[i]);
	}
	


	std::cout << "Starting while loop! \n";
	while (true)
	{


		/*sf::Packet receivedPacket;
		unsigned short packetIdentifier;
		if (receiveDataUdp(receivedPacket, data, packetIdentifier))
		{
			std::string s;
			receivedPacket >> s;
			std::cout << "Received string from packet: " << s << std::endl;
		}*/


		//std::cout << "first connection: " << data.users[0].ipAdress.toString() << " second connection: " << data.users[1].ipAdress.toString() << std::endl;


			/*sf::Packet pack;
			std::string packString = "server data XDXDDX";
			pack << packString;
			sendDataAllPlayers(pack, data);*/
		while (circBuffer->getIfPacketsLeftToRead())
		{
			int packetId = circBuffer->getPacketId();

			if (packetId == 10)
			{
				testPosition* tst = circBuffer->readData<testPosition>();
				for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
				{
					if (i == tst->playerId)
					{
						data.users[i].playa.setPosition(tst->x, tst->y, tst->z);
						break;
					}
				}
			}
		}


		if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count() > timerLength)
		{
			//för varje spelare så skicka deras position till alla klienter
			for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
			{
				testPosition pos;
				
				pos.packetId = 10;
				pos.x = data.users[i].playa.getposition('x');
				pos.y = data.users[i].playa.getposition('y');
				pos.z = data.users[i].playa.getposition('z');
				pos.playerId = i;
				
				//sending position data to all players
				//tempPack << packId << playerid << x << y << z;
				
				std::cout << "data to send x: " << std::to_string(pos.x) << ", y: " << std::to_string(pos.y) << ", z: " << std::to_string(pos.z) << std::endl;
				//std::cout << "packet id sent: " << std::to_string(pos.packetId) << std::endl;

				sendDataAllPlayers(pos, data);
			}
			start = std::chrono::system_clock::now();
		}
		
	}
	/*if (connectionType == "s")
	{
		sf::TcpListener listener;
		listener.listen(2001);
		listener.accept(socket);
		while (true)
		{
			std::getline(std::cin, s);
			packet << s;

			socket.send(packet);

			socket.receive(receivedPacket);
			if (receivedPacket >> receivedstring)
			{
				// Data extracted successfully...
				std::cout << receivedstring << std::endl;
				receivedstring.clear();
				packet.clear();
				receivedPacket.clear();
			}
		}
	}
	else if (connectionType == "c")
	{
		socket.setBlocking(false);
		socket.connect(ip, 2001);

		while (true)
		{
			std::getline(std::cin, s);
			packet << s;

			socket.send(packet);
			socket.receive(receivedPacket);
			if (receivedPacket >> receivedstring)
			{
				// Data extracted successfully...
				std::cout << receivedstring << std::endl;
				receivedstring.clear();
				packet.clear();
				receivedPacket.clear();
			}
		}
	}*/


    //(void*)getchar();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
