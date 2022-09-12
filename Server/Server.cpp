#include <iostream>
#include <iostream>
#include "../StortSpelProjekt/PackageTypes.h"

#include <SFML/Network.hpp>
#include <chrono>
#include <thread>
#include <vector>

//const int MAXNUMBEROFPLAYERS = 2;

struct acceptMePacketData
{
	std::string identifier;
	std::string s;
};

struct userData
{
	sf::IpAddress ipAdress;
	std::string userName;
	sf::TcpSocket tcpSocket;
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
	std::vector<userData> users;
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
			for (int i = 0; i < data->users.size(); i++)
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
		std::cout << "failure\n";
	}
};


void sendDataUdp(sf::Packet& sentPacket, serverData& data, std::string remoteIpAdress, unsigned short remotePort)
{
	if (data.sendSocket.send(sentPacket, remoteIpAdress, remotePort) == sf::Socket::Done)
	{
		//sent
		std::cout << "sent data to server\n";
	}
	else
	{
		std::cout << "failed to send data to server\n";
	}
};

bool receiveDataTcp(sf::Packet& receivedPacket, serverData& data, unsigned short& packetIdentifier)
{
	//remote adress

	if (data.tcpSocket.receive(receivedPacket) == sf::Socket::Done)
	{
		std::string receivedString;
		std::cout << "TCP received data from address: " << data.tcpSocket.getRemoteAddress().toString() << std::endl;

		return true;
	}
	else
	{
		std::cout << "failure\n";
	}
};

void setupTcp(serverData& data)
{
	unsigned short listenport = 2001;
	data.tcpListener.listen(listenport);
	std::cout << "listening on port: " << std::to_string((int)listenport) << std::endl;
};

void acceptPlayers(serverData& data)
{
	std::cout << "Accepting players TCP socket\n";

	int MAXNUMBEROFPLAYERS = 2;
	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		data.users.push_back(userData());
		if (data.tcpListener.accept(data.users.at(i).tcpSocket) == sf::TcpListener::Done)
		{
			std::cout << "Accepted new player ipAdress: " << data.users.at(i).tcpSocket.getRemoteAddress().toString() << std::endl;
			userData temp;
			temp.ipAdress = data.users.at(i).tcpSocket.getRemoteAddress().toString();
			temp.userName = std::to_string(i + 1);
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



	//sf::UdpSocket socket;
	std::string connectionType, mode;

	serverData data;
	userData tempUser;
	tempUser.ipAdress = "192.168.43.251";
	tempUser.userName = "mike hawk";
	sf::IpAddress remoteAddress;

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();

	data.port = 2001;

	std::cout << ip.toString() << "\n" << "port: " << data.port << std::endl;
	//serverData* serverData;
	//std::cout << "Starting handleReceiveData thread!\n";
	//std::thread* serverThread = new std::thread(handleReceivedData, &serverData);

	if (data.socket.bind(data.port) != sf::Socket::Done)
	{
		std::cout << "Failed to bind UDP servern\n";
		return -1;
	}

	std::cout << "Successfully bound socket\n";
	
	std::chrono::time_point<std::chrono::system_clock> start;
	start = std::chrono::system_clock::now();


	setupTcp(data);

	acceptPlayers(data);
	
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

		sf::Packet receivecPacket;
		unsigned short packetIdentifier;
		if (receiveDataTcp(receivecPacket, data, packetIdentifier))
		{
			std::string s;
			receivedPacket >> s;
			std::cout << "Received string from packet: " << s << std::endl;
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
