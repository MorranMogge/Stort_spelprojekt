#include "stdafx.h"
#include "SendingDataEvent.h"
#include "Component.h"
#include "PacketEnum.h"
#include "Player.h"

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
	currentPlayer->getAnimId(pr.AnimId);
	currentPlayer->getAnimSpeed(pr.animSpeed);
	client->sendStuff<PositionRotation>(pr);
	
	//holds a component
	//if (currentPlayer->getItemOnlineType() == 0)
	//{
	//	std::cout << "sent compData\n";
	//	ComponentData c;
	//	c.ComponentId = currentPlayer->getItemOnlineId();
	//	c.inUseBy = client->getPlayerId();
	//	c.packetId = PacketType::COMPONENTPOSITION;
	//	c.x = currentPlayer->getPos().x;
	//	c.y = currentPlayer->getPos().y;
	//	c.z = currentPlayer->getPos().z;
	//	client->sendStuff<ComponentData>(c);
	//}
		//hitta current players item och bara skicka den dattan ****************************************
		//currentPlayer->
		//only sends component data if the player is holding the item
		/*if (components[i]->returnStatus())
		{
			ComponentData c;
			c.ComponentId = i;
			c.inUseBy = i;
			c.packetId = PacketType::COMPONENTPOSITION;
			c.x = 69.0f;
			c.y = 60.0f;
			c.z = 69.0f;
			client->sendStuff<ComponentData>(c);
		}*/

	//ComponentData c;
	//c.ComponentId = 0;
	//c.inUseBy = -1;
	//c.packetId = PacketType::COMPONENTPOSITION;
	//c.x = 69.0f;
	//c.y = 60.0f;
	//c.z = 69.0f;
	//client->sendStuff<ComponentData>(c);

}
void DroppedComponentSendingPacket(Client*& client)
{
	//if (currentPlayer->getItemOnlineType() == -1)//doesnt hold a component
	//{
	//ComponentData c;
	//c.ComponentId = currentPlayer->getItemOnlineId();
	//c.inUseBy = client->getPlayerId();
	//c.packetId = PacketType::COMPONENTPOSITION;
	//c.x = currentPlayer->getPos().x;
	//c.y = currentPlayer->getPos().y;
	//c.z = currentPlayer->getPos().z;
	//client->sendStuff<ComponentData>(c);
	//}
}
