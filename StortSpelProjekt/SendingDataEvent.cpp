#include "stdafx.h"
#include "SendingDataEvent.h"
#include "Component.h"
#include "PacketEnum.h"

//data to send
void SendingDataEvent(Client*& client,  Player*& currentPlayer, std::vector<Player*>& players)//, std::vector<Component*>& components)
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

	
	//for (int i = 0; i < components.size(); i++)
	//{
	//	ComponentData c;
	//	c.ComponentId = i;
	//	//c.inUseBy = components[i];//om man håller component
	//	c.packetId = PacketType::COMPONENTPOSITION;
	//	c.x = 69.0f;
	//	c.y = 60.0f;
	//	c.z = 69.0f;
	//	//client->sendStuff<ComponentData>(c);
	//}
	

};