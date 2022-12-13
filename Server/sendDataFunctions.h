#include <SFML/Network.hpp>
#include "player.h"

const short MAXNUMBEROFPLAYERS = 4;

struct userData
{
	sf::IpAddress ipAdress;
	std::string userName;
	sf::TcpSocket tcpSocket;
	int playerId = -1;
	bool ifAccepted = false;
	Player playa;
};

struct serverData
{
	bool endServer = false;
	sf::UdpSocket socket;
	sf::UdpSocket sendSocket;
	sf::TcpListener tcpListener;
	sf::TcpSocket tcpSocket;
	userData users[MAXNUMBEROFPLAYERS];
	unsigned short port = 2001;
};

template <typename T>
void sendBinaryDataAllPlayers(const T& data, serverData& serverData)
{
	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		std::size_t recvSize;
		if (serverData.users[i].tcpSocket.send(&data, sizeof(T), recvSize) != sf::Socket::Done)
		{
			//error
			std::cout << "Couldnt send data to currentPlayer from array slot: " << std::to_string(i) << std::endl;
		}
		else
		{
			//std::cout << "sent data to currentPlayer: " << serverData.users[i].tcpSocket.getRemoteAddress().toString() << std::endl;
		}
	}
}

template <typename T>
void sendBinaryDataOnePlayer(const T& data, userData& user)
{
	std::size_t recvSize;
	if (user.tcpSocket.send(&data, sizeof(T), recvSize) != sf::Socket::Done)
	{
		//error
		std::cout << "Couldnt send data to currentPlayer from array slot: " << std::to_string(user.playerId) << std::endl;
	}
	else
	{
		//std::cout << "sent data to currentPlayer: " << serverData.users[i].tcpSocket.getRemoteAddress().toString() << std::endl;
	}
}