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
	std::vector<Component *>& componentVector, PhysicsWorld& physWorld, std::vector<GameObject *>& gameObjects, GravityField* field, std::vector<SpaceShip*>& spaceShips
	, std::vector<Item*>& onlineItems)
{
	//handles the online events
	idProtocol* protocol = nullptr;
	testPosition* tst = nullptr;
	ComponentData* compData = nullptr;
	SpawnComponent* spawnComp = nullptr;
	PositionRotation* prMatrixData = nullptr;
	ItemSpawn* itemSpawn = nullptr;
	itemPosition* itemPosData = nullptr;
	Component* newComponent = nullptr;
	PlayerHit* playerHit = nullptr;
	SpaceShipPosition* spaceShipPos = nullptr;
	ComponentAdded* compAdded = nullptr;
	SpaceShip* newSpaceShip = nullptr;
	Item* item = nullptr;
	BaseballBat* baseballbat = nullptr;

	while (circularBuffer->getIfPacketsLeftToRead())
	{
		int packetId = circularBuffer->peekPacketId();

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
			componentVector[compData->ComponentId]->getPhysComp()->setRotation(compData->quat);
			//std::cout << "packetHandleEvents, componentData: " << std::to_string(compData->ComponentId) << std::endl;
			break;

		case PacketType::SPAWNCOMPONENT:
			spawnComp = circularBuffer->readData<SpawnComponent>();
			std::cout << spawnComp->x << " " << spawnComp->y << " " << spawnComp->z << "\n";
			newComponent = new Component("../Meshes/component", DirectX::SimpleMath::Vector3(spawnComp->x, spawnComp->y, spawnComp->z), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
				spawnComp->ComponentId, spawnComp->ComponentId, field);
			physWorld.addPhysComponent(newComponent);
			componentVector.push_back(newComponent);
			gameObjects.push_back(newComponent);
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
						std::cout << std::to_string(prMatrixData->matrix._14) << std::endl;
						players[i]->setMatrix(prMatrixData->matrix);
					}
					else if (prMatrixData->ifDead)
					{
						std::cout << std::to_string(prMatrixData->matrix._14) << std::endl;
						players[i]->setMatrix(prMatrixData->matrix);
					}
					players[i]->getPhysComp()->setRotation(DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&prMatrixData->matrix)));
				}
			}

			break;

		case PacketType::ITEMSPAWN:

			itemSpawn = circularBuffer->readData<ItemSpawn>();
			baseballbat = new BaseballBat("../Meshes/rocket", DirectX::SimpleMath::Vector3(itemSpawn->x, itemSpawn->y, itemSpawn->z),
				DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), itemSpawn->itemId, itemSpawn->itemId, field);
			physWorld.addPhysComponent(baseballbat);
			onlineItems.push_back(baseballbat);

			std::cout << "item spawned: " << std::to_string(itemSpawn->itemId) << std::endl;
			break;

		case PacketType::ITEMPOSITION:
			itemPosData = circularBuffer->readData<itemPosition>();
			//std::cout << "item pos, item id: " << std::to_string(itemPosData->itemId) << std::endl;
			for (int i = 0; i < onlineItems.size(); i++)
			{
				//std::cout << "vector item id: " << std::to_string(onlineItems[i]->getOnlineId()) << ", recv Data itemid: " << std::to_string(itemPosData->itemId) << std::endl;
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
		case PacketType::SPACESHIPPOSITION:
			spaceShipPos = circularBuffer->readData<SpaceShipPosition>();
			//Create correct spaceship depending on team
			std::cout << "Spawned spaceship\n";
			newSpaceShip = new SpaceShip(DirectX::SimpleMath::Vector3(spaceShipPos->x, spaceShipPos->y, spaceShipPos->z), 3, spaceShipPos->spaceShipTeam, field, DirectX::SimpleMath::Vector3(2, 2, 2), 4);
			spaceShips.push_back(newSpaceShip);
			gameObjects.push_back(newSpaceShip);
			physWorld.addPhysComponent(newSpaceShip, reactphysics3d::CollisionShapeName::BOX, DirectX::XMFLOAT3(0.75f, 3 * 0.75f, 0.75f));
			newSpaceShip->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
			newSpaceShip->getPhysComp()->setRotation(DirectX::XMQuaternionRotationMatrix(newSpaceShip->getRot()));
			newSpaceShip->getPhysComp()->setPosition(reactphysics3d::Vector3(newSpaceShip->getPosV3().x, newSpaceShip->getPosV3().y, newSpaceShip->getPosV3().z));
			break;
		case PacketType::COMPONENTADDED:
			compAdded = circularBuffer->readData<ComponentAdded>();
			std::cout << "Team: " << compAdded->spaceShipTeam << " gained progress!\n";
			for (int i = 0; i < spaceShips.size(); i++)
			{
				if (i == compAdded->spaceShipTeam)
				{
					//Update hud or whatever
					spaceShips[i]->addComponent();
				}
			}
			for (int i = 0; i < players.size(); i++)
			{
				item = players[i]->getItem();
				if (item)
				{
					DirectX::SimpleMath::Vector3 randomPos = {};

					int xPos = rand() % 201 - 100;
					int yPos = rand() % 201 - 100;
					int zPos = rand() % 201 - 100;

					randomPos.x = xPos;
					randomPos.y = yPos;
					randomPos.z = zPos;

					randomPos.Normalize();
					randomPos *= 100;

					item->setPos(randomPos);
					players[i]->releaseItem();

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
