#pragma once

#include "Client.h"
#include "Player.h"

class ClientHandler
{
private:
	Client* client;

public:
	ClientHandler();

	void initializePlayers(int& nrOfplayers, int& playerId);
};