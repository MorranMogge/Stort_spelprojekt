#include "stdafx.h"
#include "PacketEventManager.h"
#include "BaseballBat.h"

PacketEventManager::PacketEventManager()
{
}

PacketEventManager::~PacketEventManager()
{
}

void PacketEventManager::PacketHandleEvents(CircularBufferClient*& circularBuffer, const int& NROFPLAYERS, std::vector<Player*>& players, const int& playerId,
	std::vector<Component*>& componentVector, PhysicsWorld& physWorld, std::vector<GameObject*>& gameObjects, GravityField* field, std::vector<SpaceShip*>& spaceShips
	, std::vector<Item*>& onlineItems, std::vector<Mesh*>& meshes, std::vector<Planet*>& planetVector, CaptureZone*& captureZone, GAMESTATE& currentGameState, Client*& client)
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
	SpawnPlanets* planetData = nullptr;
	ConfirmComponentPickedUp* confirmCmpPickedUp = nullptr;
	ComponentPosition* cmpPosition = nullptr;
	CreateZone* zonePos = nullptr;
	winner* win = nullptr;
	Loser* lose = nullptr;

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
			break;

		case PacketType::PACKETID:
			protocol = circularBuffer->readData<idProtocol>();
			std::cout << "PacketHandleEvents, received player id: " << std::to_string(protocol->assignedPlayerId) << std::endl;
			break;
		
		case PacketType::WINNER:
			win = circularBuffer->readData<winner>();

			for (int i = 0; i < 100; i++)
			{
				std::cout << "uwu i won wuw\n";
			}
			currentGameState = WIN;
			break;

		case PacketType::LOSER:
			lose = circularBuffer->readData<Loser>();

			for (int i = 0; i < 100; i++)
			{
				std::cout << "uwu i lost wuw\n";
			}
			currentGameState = LOSE;
			break;

		case PacketType::COMPONENTPOSITION:
			compData = circularBuffer->readData<ComponentData>();
			for (int i = 0; i < componentVector.size(); i++)
			{
				if (componentVector[i]->getOnlineId() == compData->ComponentId)
				{
					componentVector[i]->setPos(DirectX::XMFLOAT3(compData->x, compData->y, compData->z));
					//onlineItems[i]->getPhysComp()->setRotation(compData->quat);
				}
			}
			//std::cout << "packetHandleEvents, componentData: " << std::to_string(compData->ComponentId) << std::endl;
			break;

		case PacketType::SPAWNCOMPONENT:
			spawnComp = circularBuffer->readData<SpawnComponent>();
			
			//std::cout << "Comp ID: " << spawnComp->ComponentId << "\n";
			
			newComponent = new Component(meshes[6], DirectX::SimpleMath::Vector3(spawnComp->x, spawnComp->y, spawnComp->z), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
				COMPONENT, spawnComp->ComponentId, field);
			physWorld.addPhysComponent(newComponent);
			onlineItems.push_back(newComponent);
			//gameObjects.push_back(newComponent);
			//componentVector.push_back(newComponent);
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
			baseballbat = new BaseballBat(meshes[5], DirectX::SimpleMath::Vector3(itemSpawn->x, itemSpawn->y, itemSpawn->z),
				DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 3, itemSpawn->itemId, field);
			baseballbat->setClient(client);
			baseballbat->setGameObjects(players);
			physWorld.addPhysComponent(baseballbat);
			onlineItems.push_back(baseballbat);
			//gameObjects.push_back(baseballbat);
			std::cout << "item spawned UWU: " << std::to_string(itemSpawn->itemId) << std::endl;
			break;

		case PacketType::ITEMPOSITION:
			itemPosData = circularBuffer->readData<itemPosition>();
			//std::cout << "item pos, item id: " << std::to_string(itemPosData->itemId) << std::endl;
			for (int i = 0; i < componentVector.size(); i++)
			{
				//std::cout << "vector item id: " << std::to_string(componentVector[i]->getOnlineId()) << ", recv Data itemid: " << std::to_string(itemPosData->itemId) << std::endl;
				if (componentVector[i]->getOnlineId() == itemPosData->itemId)
				{
					componentVector[i]->setPos(DirectX::XMFLOAT3(itemPosData->x, itemPosData->y, itemPosData->z));
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
			newSpaceShip = new SpaceShip(meshes[4], DirectX::SimpleMath::Vector3(spaceShipPos->x, spaceShipPos->y, spaceShipPos->z), 3, spaceShipPos->spaceShipTeam, field, meshes[9], DirectX::SimpleMath::Vector3(2, 2, 2), 4);
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
				if (spaceShips[i]->getTeam() == compAdded->spaceShipTeam)
				{
					//Update hud or whatever
					spaceShips[i]->addComponent();
					spaceShips[i]->setAnimate(true);
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

					randomPos.x = (float)xPos;
					randomPos.y = (float)yPos;
					randomPos.z = (float)zPos;

					randomPos.Normalize();
					randomPos *= 100;

					item->setPos(randomPos);
					players[i]->releaseItem();

				}
			}
			break;

		case PacketType::SPAWNPLANETS:
			planetData = circularBuffer->readData<SpawnPlanets>();
			std::cout << "Received planet\n";
			planetVector.emplace_back(new Planet(meshes[0], DirectX::XMFLOAT3(planetData->size, planetData->size, planetData->size), DirectX::XMFLOAT3(planetData->xPos, planetData->yPos, planetData->zPos)));
			planetVector.back()->setPlanetShape(&physWorld);
			physWorld.setPlanets(planetVector);
			break;

		case PacketType::COMPONENTCONFIRMEDPICKUP:
			confirmCmpPickedUp = circularBuffer->readData<ConfirmComponentPickedUp>();
			for (int i = 0; i < players.size(); i++)
			{
				if (players[i]->getOnlineID() == confirmCmpPickedUp->playerPickUpId)
				{
					//en av spelarna plockade upp en component
					//std::cout << "Confirmed pickup recv comId: " << std::to_string(confirmCmpPickedUp->componentId) << std::endl;

					for (int j = 0; j < onlineItems.size(); j++)
					{
						if (confirmCmpPickedUp->componentId == onlineItems[j]->getOnlineId())
						{
							std::cout << "ConfirmPacket, picked up component Id: " << std::to_string(confirmCmpPickedUp->componentId) << std::endl;
							players[i]->itemRecvFromServer(onlineItems[j]);
							break;
						}
					}
				}
			}
			std::cout << "Done with it uwu\n";
			break;

		case PacketType::COMPONENTPOSITIONNEW:
			cmpPosition = circularBuffer->readData<ComponentPosition>();

			for (int i = 0; i < onlineItems.size(); i++)
			{
				//std::cout << "comp Id: " << std::to_string(cmpPosition->ComponentId) << ", i = " << std::to_string(i) << std::endl;
				if (onlineItems[i]->getOnlineId() == cmpPosition->ComponentId)
				{
					//std::cout << "comp Id: " << std::to_string(cmpPosition->ComponentId) << ", pos x: " << std::to_string(cmpPosition->x)
						//<< ", y: " << std::to_string(cmpPosition->y) << std::endl;


					onlineItems[i]->setPos(DirectX::XMFLOAT3(cmpPosition->x, cmpPosition->y, cmpPosition->z));
					//componentVector[i]->getPhysComp()->setRotation(cmpPosition->quat);
				}
			}
			//std::cout << "packetHandleEvents, componentData: " << std::to_string(compData->ComponentId) << std::endl;
			break;

		case PacketType::CREATEZONE:
			zonePos = circularBuffer->readData<CreateZone>();
			std::cout << "uwuw zone done\n";
			captureZone = new CaptureZone(meshes[9], DirectX::SimpleMath::Vector3(zonePos->xPos, zonePos->yPos, zonePos->zPos), DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f), field, DirectX::SimpleMath::Vector3(zonePos->scale, zonePos->scale, zonePos->scale),{0,1,0});
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
