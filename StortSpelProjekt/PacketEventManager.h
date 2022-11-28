#pragma once
#include "PacketEnum.h"
#include "Player.h"
#include "Component.h"
#include "PhysicsWorld.h"
#include "PacketsDataTypes.h"
#include "CircularBufferClient.h"
#include "SpaceShip.h"
#include "State.h"

class PacketEventManager
{
private:

public:
	PacketEventManager();
	~PacketEventManager();

	void PacketHandleEvents(CircularBufferClient*& circularBuffer, const int& NROFPLAYERS, std::vector<Player*>& players, const int& playerId,
		std::vector<Component*>& compnentVector, PhysicsWorld& physWorld, std::vector<GameObject*>& gameObjects,
		GravityField* field, std::vector<SpaceShip*>& spaceShips, std::vector<Item*>& onlineItems, std::vector<Mesh*>& meshes, std::vector<Planet*>& planetVector, CaptureZone*& captureZone, GAMESTATE& currentGameState, Client*& client);
	int handleId(CircularBufferClient*& circularBuffer);
};