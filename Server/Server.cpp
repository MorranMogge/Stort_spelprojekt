#include <iostream>
#include <string>
#include "../StortSpelProjekt/PackageTypes.h"

#include <SFML/Network.hpp>
#include <chrono>
#include <thread>
#include <vector>
#include "player.h"

const short MAXNUMBEROFPLAYERS = 1;

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

void sendDataAllPlayers(sf::Packet& packet, serverData& data)
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

void recvData(void* param, userData* user)
{
	threadInfo* data = (threadInfo*)param;
	sf::Packet packet;

	std::cout << "ip from socket in thread: " << user->tcpSocket.getRemoteAddress().toString() << std::endl;
	while (1)
	{
		if (user->tcpSocket.receive(packet) != sf::Socket::Done)
		{
			//error
		}
		else
		{
			//checks what id the packet is
			int packetId = extractPacketId(packet);
			std::cout << "thread from recvData got a packet from: " << user->tcpSocket.getRemoteAddress().toString() << ", Packet id: " << std::to_string(packetId) << std::endl;

			//if id is 3, player sets position 
			if (packetId == 3)
			{
				packet >> data->pos[0] >> data->pos[1] >> data->pos[2];
			}
		}
		
		//std::cout << "userName: " << data->user->userName << std::endl;
	}
};

void sendIdToAllPlayers(serverData& data)
{
	unsigned short packetid = 10;

	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		sf::Packet pack;
		pack << packetid << data.users[i].playerId;

		if (data.users[i].tcpSocket.send(pack) != sf::Socket::Done)
		{
			//error
			std::cout << "sendIdToAllPlayers(), couldnt send id to player: " << std::to_string(i) << std::endl;
		}
		else
		{
			std::cout << "sendIdToAllPlayers() sent to player nr: " << std::to_string(data.users[i].playerId);
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



	std::thread* recvThread[MAXNUMBEROFPLAYERS];
	threadInfo threadData[MAXNUMBEROFPLAYERS];
	//threadData.ifUserRecv = false;
	
	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		threadData[i].pos[0] = 0.0f;
		threadData[i].pos[1] = 0.0f;
		threadData[i].pos[2] = 0.0f;
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
		for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
		{
			data.users[i].playa.setPosition(threadData[i].pos);
		}

		if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count() > timerLength)
		{
			//f�r varje spelare s� skicka deras position till alla klienter
			for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
			{
				sf::Packet tempPack;
				float x = data.users[i].playa.getposition('x');
				float y = data.users[i].playa.getposition('y');
				float z = data.users[i].playa.getposition('z');
				int playerid = data.users[i].playerId;
				unsigned short packId = 3;
				//sending position data to all players
				tempPack << packId << playerid << x << y << z;
				
				sendDataAllPlayers(tempPack, data);
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
