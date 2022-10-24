#include "stdafx.h"
#include "SendingDataEvent.h"
#include "PacketEnum.h"

//data to send
void SendingDataEvent(Client*& client,  Player*& currentPlayer, std::vector<Player*>& players)
{
	testPosition t;
	t.packetId = PacketType::POSITION;
	t.playerId = client->getPlayerId();
	t.x = currentPlayer->getPos().x;
	t.y = currentPlayer->getPos().y;
	t.z = currentPlayer->getPos().z;
	client->sendStuff<testPosition>(t);

	//send data here 
};