#include <iostream>
#include <iostream>
#include "../StortSpelProjekt/PackageTypes.h"

#include <SFML/Network.hpp>
#include <chrono>
#include <thread>
#include <vector>

struct userData
{
	sf::IpAddress ipAdress;
	std::string userName;
};

struct serverData
{
	bool endServer = false;
	sf::UdpSocket socket;
	std::vector< userData> users;
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
		if (data->socket.receive(packet, remoteAddress, data->port))
		{
			for (int i = 0; i < data->users.size(); i++)
			{
				if (data->users[i].ipAdress == remoteAddress)
				{
					std::cout << "received data from address: " << remoteAddress.toString() << std::endl;

					data->packet = packet;
					packet.clear();
				}
			}
		}
	}

}


int main()
{
	std::string s = "empty";
	// Group the variables to send into a packet
	sf::Packet packet;
	sf::Packet receivedPacket;
	float frequency = 30.f;

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	std::cout << ip.toString() << "\n";

	sf::UdpSocket socket;
	std::string connectionType, mode;

	serverData* serverData;

	std::cout << "Starting handleReceiveData thread!\n";
	std::thread* serverThread = new std::thread(handleReceivedData, &serverData);

	std::cout << "Starting server!\n";
	
	std::chrono::time_point<std::chrono::system_clock> start;
	start = std::chrono::system_clock::now();

	int counter = 0;
	while (true)
	{
		if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count() > 1.0f / frequency)
		{
			std::getline(std::cin, s);
			//s = std::to_string(counter++);
			packet << s;

			
			packet.clear();
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


    getchar();
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
