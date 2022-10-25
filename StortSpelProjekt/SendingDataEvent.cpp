#include "stdafx.h"
#include "SendingDataEvent.h"
#include "PacketEnum.h"

//data to send
void SendingDataEvent(Client*& client,  Player*& currentPlayer, std::vector<Player*>& players)
{
	//send data here 
	testPosition t;
	t.packetId = PacketType::POSITION;
	t.playerId = client->getPlayerId();
	t.x = currentPlayer->getPos().x;
	t.y = currentPlayer->getPos().y;
	t.z = currentPlayer->getPos().z;
	client->sendStuff<testPosition>(t);

	PositionRotation pr;
	pr.packetId = PacketType::POSITIONROTATION;
	pr.playerId = client->getPlayerId();
	pr.matrix = currentPlayer->getMatrix();
	client->sendStuff<PositionRotation>(pr);

	//ComponentData c;
	//c.ComponentId = 0;
	//c.inUseBy = -1;
	//c.packetId = PacketType::COMPONENTPOSITION;
	//c.x = 69.0f;
	//c.y = 60.0f;
	//c.z = 69.0f;
	//client->sendStuff<ComponentData>(c);

};