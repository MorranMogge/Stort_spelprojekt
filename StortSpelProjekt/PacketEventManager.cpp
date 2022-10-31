#include "stdafx.h"
#include "PacketEventManager.h"
#include "BaseballBat.h"


PacketEventManager::PacketEventManager()
{
}

PacketEventManager::~PacketEventManager()
{
}

void PacketEventManager::PacketHandleEvents(CircularBufferClient*& circularBuffer, const int &NROFPLAYERS, std::vector<Player*>& players, const int& playerId,
	std::vector<Component *>& componentVector, PhysicsWorld& physWorld, std::vector<Item*>&onlineItems)
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
		ItemSpawn* itemSpawn = nullptr;
		itemPosition* itemPosData = nullptr;
		Component* newComponent = nullptr;
		PlayerHit* playerHit = nullptr;
		BaseballBat* newBaseballItem = nullptr;

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
			componentVector[compData->ComponentId]->setPos(DirectX::XMFLOAT3(compData->x, compData->y, compData->z));
			//std::cout << "packetHandleEvents, componentData: " << std::to_string(compData->ComponentId) << std::endl;
			break;

		case PacketType::SPAWNCOMPONENT:
			spawnComp = circularBuffer->readData<SpawnComponent>();
			std::cout << spawnComp->x << " " << spawnComp->y << " " << spawnComp->z << "\n";
			newComponent = new Component("../Meshes/Player", DirectX::SimpleMath::Vector3(spawnComp->x, spawnComp->y, spawnComp->z), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), spawnComp->ComponentId, spawnComp->ComponentId);
			physWorld.addPhysComponent(newComponent);
			componentVector.push_back(newComponent);
 			std::cout << "Sucessfully recieved component from server: " << std::to_string(spawnComp->ComponentId) << std::endl;
			break;
		
		case PacketType::POSITIONROTATION:
			prMatrixData = circularBuffer->readData<PositionRotation>();

			for (int i = 0; i < players.size(); i++)
			{
				//std::cout << std::to_string(players[i]->getMatrix()._14) << std::endl;
				
				if (prMatrixData->playerId == i)
				{
					if (playerId != i)
					{
						//std::cout << std::to_string(prMatrixData->matrix._14) << std::endl;
						players[i]->setMatrix(prMatrixData->matrix);
					}
					else if (prMatrixData->ifDead)
					{
						//std::cout << std::to_string(prMatrixData->matrix._14) << std::endl;
						players[i]->setMatrix(prMatrixData->matrix);
					}
					//players[i]->getPhysComp()->setRotation(DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&prMatrixData->matrix)));
				}
			}

			break;

		case PacketType::ITEMSPAWN:

			itemSpawn = circularBuffer->readData<ItemSpawn>();
			newBaseballItem = new BaseballBat("../Meshes/Player", DirectX::SimpleMath::Vector3(itemSpawn->x, itemSpawn->y, itemSpawn->z),
			DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), itemSpawn->itemId, itemSpawn->itemId);
			physWorld.addPhysComponent(newBaseballItem);
			onlineItems.push_back(newBaseballItem);

			std::cout << "item spawned: " << std::to_string(itemSpawn->itemId) << std::endl;
			break;

		case PacketType::ITEMPOSITION:
			itemPosData = circularBuffer->readData<itemPosition>();
			//std::cout << "item pos, item id: " << std::to_string(itemPosData->itemId) << std::endl;
			for (int i = 0; i < onlineItems.size(); i++)
			{
				if (onlineItems[i]->getOnlineId() == itemPosData->itemId)
				{
					onlineItems[i]->setPos(DirectX::XMFLOAT3(itemPosData->x, itemPosData->y, itemPosData->z));
					break;
				}
				
			}
			break;
		
		case PacketType::PLAYERHIT:
			playerHit = circularBuffer->readData<PlayerHit>();
			if (playerHit->playerId == playerId) players[playerId]->hitByBat(reactphysics3d::Vector3(playerHit->xForce, playerHit->yForce, playerHit->zForce));
			break;

		case PacketType::WINNER:

				break;
		case PacketType::LOSER:

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
