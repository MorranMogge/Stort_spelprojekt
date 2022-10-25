#include "stdafx.h"
#include "ClientHandler.h"

ClientHandler::ClientHandler()
{
}

void ClientHandler::initializePlayers(int& nrOfplayers, int& playerId)
{
	client->connectToServer("192.168.43.251", 2001);
	std::cout << "connected to server\n";
	//need to recv player id
}
