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

struct
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

	player playa;
	//userData()
	//{

	//}
	//userData(const userData& obj)
	//{
	//	this->ipAdress == obj.ipAdress;
	//	this->userName = obj.userName;
	//	this->tcpSocket = tcpSocket;
	//}
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
void handleReceivedData(void* param)
{
	serverData* data = (serverData*)param;
	sf::Packet packet;
	sf::IpAddress remoteAddress;

	while (!data->endServer)
	{
		if (data->socket.receive(packet, data->users[0].ipAdress, data->port))
		{
			for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
			{
				if (data->users[i].ipAdress == remoteAddress)
				{
					std::string receivedString;
					std::cout << "received data from address: " << remoteAddress.toString() << std::endl;

					packet >> receivedString;
					std::cout << "Received string from client: " << receivedString << std::endl;

					data->packet = packet;
					packet.clear();
				}
			}
		}
	}

};

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
			data.users[i].userName = std::to_string(i + 1);
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

void packetId(sf::Packet& packet, serverData& data)
{
	//sendPacket >> id >> n >> s;
	unsigned short packetIdentifier;
	int nr;
	std::string s;

	packet >> packetIdentifier;
	std::cout << "packetId: " << std::to_string(packetIdentifier) << std::endl;

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


	sf::SocketSelector selector;

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

	//data.users->tcpSocket.send(&data, sizeof(data));

		
	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		selector.add(data.users[i].tcpSocket);
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

		if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count() > timerLength)
		{
			//för varje spelare så skicka deras position till alla klienter
			std::cout << "all that\n";
			for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
			{

				sf::Packet tempPack;
				float xwow = data.users[i].playa.getposition('x');
				float ywow = data.users[i].playa.getposition('y');
				std::string playerName = "player " + std::to_string(i);
				tempPack << playerName << xwow << ywow;
				std::cout << "Sent to all players\n";
				sendDataAllPlayers(tempPack, data);
			}
			start = std::chrono::system_clock::now();
		}

	/*	if (selector.wait())
		{
			for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
			{
				if (selector.isReady(data.users[i].tcpSocket))
				{
					std::cout << "user ip: " << data.users[i].ipAdress.toString() << std::endl;
					sf::Packet receivedPacket;
					unsigned short packetIdentifier;

					if (receiveDataTcp(receivedPacket, data, i))
					{
						packetId(receivedPacket, data);
					}
				}
			}
		}*/
		
		
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
