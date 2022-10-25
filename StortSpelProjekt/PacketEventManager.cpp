#include "stdafx.h"
#include "PacketEventManager.h"

PacketEventManager::PacketEventManager()
{
}

PacketEventManager::~PacketEventManager()
{
}

void PacketEventManager::PacketHandleEvents(CircularBufferClient*& circularBuffer, const int &NROFPLAYERS, std::vector<Player*>& players, const int& playerId)
{
	//handles the online events
	while (circularBuffer->getIfPacketsLeftToRead())
	{
		int packetId = circularBuffer->peekPacketId();

		idProtocol* protocol = nullptr;
		testPosition* tst = nullptr;
		ComponentData* compData = nullptr;
		SpawnComponent* spawnComp = nullptr;
		PositionRotation* prMatrixData = nullptr;

		switch (packetId)
		{
		default:
			break;

		case PacketType::POSITION:

			tst = circularBuffer->readData<testPosition>();
			for (int i = 0; i < NROFPLAYERS; i++)
			{
				if (i == tst->playerId)
				{
					if (playerId != i)
					{
						players[i]->setPos(DirectX::XMFLOAT3(tst->x, tst->y, tst->z));
					}
				}
			}

			//std::cout << "Checking return value from circular buffer player id.x: " << std::to_string(tst->playerId) << " y: " << std::to_string(tst->y) << std::endl;
			break;

		case PacketType::PACKETID:

			protocol = circularBuffer->readData<idProtocol>();
			std::cout << "PacketHandleEvents, received player id: " << std::to_string(protocol->assignedPlayerId) << std::endl;
			break;

		case PacketType::COMPONENTPOSITION:

			compData = circularBuffer->readData<ComponentData>();
			std::cout << "packetHandleEvents, componentData: " << std::to_string(compData->ComponentId) << std::endl;
			break;

		case PacketType::SPAWNCOMPONENT:
			spawnComp = circularBuffer->readData<SpawnComponent>();
			std::cout << "Received SpawnComponent id: " << std::to_string(spawnComp->ComponentId) << std::endl;
			break;
		
		case PacketType::POSITIONROTATION:
			prMatrixData = circularBuffer->readData<PositionRotation>();
			for (int i = 0; i < players.size(); i++)
			{
				if (playerId != i)
				{
					players[i]->setMatrix(prMatrixData->matrix);
				}
			}

			break;
		}
	}
}

int PacketEventManager::handleId(CircularBufferClient*& circularBuffer)
{
	while (circularBuffer->getIfPacketsLeftToRead())
	{
		int packetId = circularBuffer->peekPacketId();

		std::cout << "Packet id: " << std::to_string(packetId) << std::endl;

		idProtocol* protocol = nullptr;
		testPosition* tst = nullptr;
		ComponentData* compData = nullptr;
		SpawnComponent* spawnComp = nullptr;

		switch (packetId)
		{
		default:
			break;
		case PacketType::PACKETID:

			protocol = circularBuffer->readData<idProtocol>();
			std::cout << "received player id: " << std::to_string(protocol->assignedPlayerId) << std::endl;
			return protocol->assignedPlayerId;
			break;

		case PacketType::POSITION:

			tst = circularBuffer->readData<testPosition>();

			std::cout << "throwing away position x: " << std::to_string(tst->playerId) << " y: " << std::to_string(tst->y) << std::endl;
			return -2;
			break;

		case PacketType::COMPONENTPOSITION:

			compData = circularBuffer->readData<ComponentData>();
			std::cout << "packetHandleEvents, componentData: " << std::to_string(compData->ComponentId) << std::endl;
			break;

		case PacketType::SPAWNCOMPONENT:
			spawnComp = circularBuffer->readData<SpawnComponent>();
			std::cout << "Received SpawnComponent id: " << std::to_string(spawnComp->ComponentId) << std::endl;
			break;
		}

	}

	return -1;
}
