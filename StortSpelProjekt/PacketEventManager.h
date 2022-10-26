#pragma once
#include"PacketEnum.h"
#include"Player.h"
#include "Component.h"
#include "PhysicsWorld.h"
#include"PacketsDataTypes.h"
#include"CircularBufferClient.h"

class PacketEventManager
{
private:

public:
	PacketEventManager();
	~PacketEventManager();

	void PacketHandleEvents(CircularBufferClient*& circularBuffer, const int& NROFPLAYERS, std::vector<Player*>& players, const int& playerId, std::vector<Component*>& compnentVector, PhysicsWorld& physWorld);
	int handleId(CircularBufferClient*& circularBuffer);
};