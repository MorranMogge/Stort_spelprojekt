#include "stdafx.h"
#include "PacketEventManager.h"
#include "BaseballBat.h"
#include "Grenade.h"

PacketEventManager::PacketEventManager()
{
	for (int i = 0; i < 16; i++)
	{
		animated.push_back(false);
	}
}

PacketEventManager::~PacketEventManager()
{
}

void PacketEventManager::PacketHandleEvents(CircularBufferClient*& circularBuffer, const int& NROFPLAYERS, std::vector<Player*>& players, const int& playerId,
	std::vector<Component*>& componentVector, PhysicsWorld& physWorld, std::vector<GameObject*>& gameObjects,
	GravityField* field, std::vector<SpaceShip*>& spaceShips, std::vector<Item*>& onlineItems, std::vector<Mesh*>& meshes,
	std::vector<Planet*>& planetVector, CaptureZone*& captureZone, MiniGames& currentMinigame, float& redTeamPoints, float& blueTeamPoints, Client*& client, const float dt, GAMESTATE& currentGameState, DirectX::GamePad* gamePad)
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
	Grenade* grenade = nullptr;
	Potion* potion = nullptr;
	SpawnPlanets* planetData = nullptr;
	ConfirmComponentPickedUp* confirmCmpPickedUp = nullptr;
	ComponentPosition* cmpPosition = nullptr;
	CreateZone* zonePos = nullptr;
	MinigameStart* startMinigame = nullptr;
	LandingMiniGameScore* landingMiniGameScore = nullptr;
	winner* win = nullptr;
	Loser* lose = nullptr;
	ComponentDropped* cmpDropped = nullptr;
	HitByGrenade* hitByGrenade = nullptr;
	UseGrenade* grenadeData = nullptr;
	KTHPoints* kthPoints = nullptr;

	for (int i = 0; i < players.size(); i++)
	{
		animated[i] = false;
	}

	while (circularBuffer->getIfPacketsLeftToRead())
	{
		int packetId = circularBuffer->peekPacketId();
		
		switch (packetId)
		{
		default:
			circularBuffer->clearBuffer();
			std::cout << "RECEIVED A BAD PACKET, CLEARED THE CIRCLE BUFFER PacketEventManager.cpp\nPacketID: " << packetId << "\n";
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
			currentGameState = WIN;
			break;

		case PacketType::LOSER:
			lose = circularBuffer->readData<Loser>();
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
			newComponent->setGamepad(gamePad);
			physWorld.addPhysComponent(newComponent);
			onlineItems.push_back(newComponent);
			gameObjects.push_back(newComponent);
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
						players[i]->setMatrix(prMatrixData->matrix);
						players[i]->setRot(DirectX::SimpleMath::Quaternion(prMatrixData->xRot, prMatrixData->yRot, prMatrixData->zRot, prMatrixData->wRot));
						if (!animated[i])
						{
							players[i]->updateAnim(0.034*0.5f, prMatrixData->AnimId, 1);
							animated[i] = true;
						}
					}
					else if (prMatrixData->ifDead)
					{
						players[i]->setMatrix(prMatrixData->matrix);
						players[i]->setRot(DirectX::SimpleMath::Quaternion(prMatrixData->xRot, prMatrixData->yRot, prMatrixData->zRot, prMatrixData->wRot));

					}
					players[i]->getPhysComp()->setRotation(DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&prMatrixData->matrix)));
				}
			}
			break;

		case PacketType::ITEMSPAWN:
			itemSpawn = circularBuffer->readData<ItemSpawn>();
			std::cout << "OBJID: " << itemSpawn->itemType << "\n";
			if (itemSpawn->itemType == ObjID::BAT)
			{
				baseballbat = new BaseballBat(meshes[5], DirectX::SimpleMath::Vector3(itemSpawn->x, itemSpawn->y, itemSpawn->z),
					DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), ObjID::BAT, itemSpawn->itemId, field);
				baseballbat->setClient(client);
				baseballbat->setGameObjects(players);
				baseballbat->setGamepad(gamePad);
				physWorld.addPhysComponent(baseballbat);
				onlineItems.push_back(baseballbat);
				gameObjects.push_back(baseballbat);

			}
			else if (itemSpawn->itemType == ObjID::POTION)
			{
				potion = new Potion(meshes[3], DirectX::SimpleMath::Vector3(itemSpawn->x, itemSpawn->y, itemSpawn->z),
					DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), ObjID::POTION, itemSpawn->itemId, field);
				//grenade->setClient(client);
				potion->setGamepad(gamePad);
				physWorld.addPhysComponent(potion);
				onlineItems.push_back(potion);
				gameObjects.push_back(potion);
			}
			else if (itemSpawn->itemType == ObjID::GRENADE)
			{
				grenade = new Grenade(meshes[7], meshes[0], DirectX::SimpleMath::Vector3(itemSpawn->x, itemSpawn->y, itemSpawn->z),
					DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), ObjID::GRENADE, itemSpawn->itemId, field);
				//grenade->setClient(client);
				grenade->setGameObjects(gameObjects);
				grenade->setGamepad(gamePad);
				physWorld.addPhysComponent(grenade);
				onlineItems.push_back(grenade);
				gameObjects.push_back(grenade);
			}
			//gameObjects.push_back(baseballbat);
			//std::cout << "item spawned UWU: " << std::to_string(itemSpawn->itemId) << std::endl;
			//std::cout << "SIZE ITEMS " << onlineItems.size() << "\n";
			break;

		case PacketType::ITEMPOSITION:
			itemPosData = circularBuffer->readData<itemPosition>();
			//std::cout << "item pos, item id: " << std::to_string(itemPosData->itemId) << std::endl;
			for (int i = 0; i < onlineItems.size(); i++)
			{
				//std::cout << "vector item id: " << std::to_string(componentVector[i]->getOnlineId()) << ", recv Data itemid: " << std::to_string(itemPosData->itemId) << std::endl;
				if (onlineItems[i]->getOnlineId() == itemPosData->itemId)
				{
					onlineItems[i]->setPos(DirectX::XMFLOAT3(itemPosData->x, itemPosData->y, itemPosData->z));
					break;
				}
			}
			break;

		case PacketType::PLAYERHIT:
			playerHit = circularBuffer->readData<PlayerHit>();
			if (playerHit->playerId == playerId)
			{
				
				players[playerId]->hitByBat(reactphysics3d::Vector3(playerHit->xForce, playerHit->yForce, playerHit->zForce));
			}
			break;

		case PacketType::SPACESHIPPOSITION:
			spaceShipPos = circularBuffer->readData<SpaceShipPosition>();
			//Create correct spaceship depending on team
			std::cout << "Spawned spaceship\n";
			if (spaceShips.size() == 0)
			{
				newSpaceShip = new SpaceShip(meshes[10], DirectX::SimpleMath::Vector3(spaceShipPos->x, spaceShipPos->y, spaceShipPos->z), 3, spaceShipPos->spaceShipTeam, field, meshes[9], DirectX::SimpleMath::Vector3(2, 2, 2), 4);
			}
			else
			{
				newSpaceShip = new SpaceShip(meshes[11], DirectX::SimpleMath::Vector3(spaceShipPos->x, spaceShipPos->y, spaceShipPos->z), 3, spaceShipPos->spaceShipTeam, field, meshes[9], DirectX::SimpleMath::Vector3(2, 2, 2), 4);
			}
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
					spaceShips[i]->setAnimate(true); //CHANGE BACK WHEN DONE!
				}
			}
			for (int i = 0; i < players.size(); i++)
			{
				item = players[i]->getItem();
				if (item && item->getOnlineId() == compAdded->componentID)
				{
					players[i]->releaseItem(); //REMOVE THIS
				}
			}
			break;

		case PacketType::SPAWNPLANETS:
			planetData = circularBuffer->readData<SpawnPlanets>();
			std::cout << "Received planet\n";

			if (planetVector.size() == 3)
			{
				planetCounter %= 3;
				planetVector[planetCounter]->setPosition(DirectX::XMFLOAT3(planetData->xPos, planetData->yPos, planetData->zPos));
				planetVector[planetCounter]->setScale(DirectX::XMFLOAT3(planetData->size, planetData->size, planetData->size));
				physWorld.setPlanets(planetVector);
				planetCounter++;
			}
			else
			{
				planetVector.emplace_back(new Planet(meshes[0], DirectX::XMFLOAT3(planetData->size, planetData->size, planetData->size), DirectX::XMFLOAT3(planetData->xPos, planetData->yPos, planetData->zPos), (4.0f * 9.82f), meshes[1]));
				planetVector.back()->setPlanetShape(&physWorld);
				physWorld.setPlanets(planetVector);
			}
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
					//	<< ", y: " << std::to_string(cmpPosition->y) << std::endl;
					if (cmpPosition->playerOnlineId == -1)
					{
						onlineItems[i]->setPos(DirectX::XMFLOAT3(cmpPosition->x, cmpPosition->y, cmpPosition->z));
						onlineItems[i]->getPhysComp()->setRotation(DirectX::SimpleMath::Quaternion(cmpPosition->xRot, cmpPosition->yRot, cmpPosition->zRot, cmpPosition->wRot));
					}
					else
					{
						for (int j = 0; j < players.size(); j++)
						{
							if (players[j]->getOnlineID() == cmpPosition->playerOnlineId)
							{
								DirectX::XMFLOAT4X4 f4;
								players[j]->forwardKinematics("hand3:hand3:RightHand", f4);
								DirectX::XMMATRIX mat = DirectX::XMLoadFloat4x4(&f4);
								DirectX::XMVECTOR scale;
								DirectX::XMVECTOR pos;
								DirectX::XMVECTOR rot;
								DirectX::XMMatrixDecompose(&scale, &rot, &pos, mat);
								DirectX::SimpleMath::Vector3 newPos = pos;

								//newPos += 4 * forwardVector;

								PhysicsComponent* itemPhysComp = onlineItems[i]->getPhysComp();
								onlineItems[i]->setPos(newPos);
								onlineItems[i]->setRot(rot);
								itemPhysComp->setRotation(rot);
								itemPhysComp->setPosition(reactphysics3d::Vector3({ newPos.x, newPos.y, newPos.z }));
							}
						}
					}
				}
			}
			//std::cout << "packetHandleEvents, componentData: " << std::to_string(compData->ComponentId) << std::endl;
			break;
		case PacketType::LANDINGMINIGAMESCORE:
			landingMiniGameScore = circularBuffer->readData<LandingMiniGameScore>();
			if (players[playerId]->getTeam() == 0)
			{
				blueTeamPoints = landingMiniGameScore->pointsBlueTeam;
				redTeamPoints = landingMiniGameScore->pointsRedTeam;
			}
			else
			{
				blueTeamPoints = landingMiniGameScore->pointsRedTeam;
				redTeamPoints = landingMiniGameScore->pointsBlueTeam;
			}
			break;

		case PacketType::CREATEZONE:
			zonePos = circularBuffer->readData<CreateZone>();
			captureZone = new CaptureZone(meshes[9], DirectX::SimpleMath::Vector3(zonePos->xPos, zonePos->yPos, zonePos->zPos), DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f), field, DirectX::SimpleMath::Vector3(zonePos->scale, zonePos->scale, zonePos->scale));
			captureZone->setColor(DirectX::SimpleMath::Vector3(1.f, 1.f, 1.f));
			break;

		case PacketType::STARTMINIGAMES:
			startMinigame = circularBuffer->readData<MinigameStart>();
			currentMinigame = startMinigame->minigame;
			std::cout << "RECEIVED START OF MINIGAME\n";
			break;

		case PacketType::COMPONENTDROPPED:
			cmpDropped = circularBuffer->readData<ComponentDropped>();
			
			for (int i = 0; i < players.size(); i++)
			{
				if (players[i]->getOnlineID() == cmpDropped->playerId)
				{
					//std::cout << "playerID UWU TEst:" << cmpDropped->playerId << std::endl;
					//std::cout << "x: " << onlineItems[cmpDropped->componentId]->getPos().x << "y: " << onlineItems[cmpDropped->componentId]->getPos().y << std::endl;
					players[i]->releaseItem();
				}
			}
			break;

		case PacketType::HITBYGRENADE:
			hitByGrenade = circularBuffer->readData<HitByGrenade>();
			players[playerId]->hitByBat(reactphysics3d::Vector3(hitByGrenade->xForce, hitByGrenade->yForce, hitByGrenade->zForce));

			break;
		case PacketType::USEGRENADE:
			grenadeData = circularBuffer->readData<UseGrenade>();
			for (int i = 0; i < onlineItems.size(); i++)
			{
				if (onlineItems[i]->getOnlineId() == grenadeData->itemId)
				{
					onlineItems[i]->useItem(players[playerId]);
				}
			}
			break;

		case PacketType::KTHPOINTS:
			kthPoints = circularBuffer->readData<KTHPoints>();

			//Seeing what color the capture zone should be
			if (kthPoints->teamColor == 0) captureZone->setColor(DirectX::SimpleMath::Vector3(1.f, 0.f, 1.f));
			else if (kthPoints->teamColor == 1) captureZone->setColor(DirectX::SimpleMath::Vector3(0.f, 0.f, 1.f));
			else if (kthPoints->teamColor == 2) captureZone->setColor(DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f));
			else if (kthPoints->teamColor == 3) captureZone->setColor(DirectX::SimpleMath::Vector3(1.f, 1.f, 1.f));
			break;

		}
	}
}

int PacketEventManager::handleId(CircularBufferClient*& circularBuffer, std::vector<Planet*>& planetVector, PhysicsWorld& physWorld, std::vector<Mesh*>& meshes,
	std::vector<SpaceShip*>& spaceShips, std::vector<GameObject*>& gameObjects, GravityField* field, int& UwU)
{

	SpawnPlanets* planetData = nullptr;
	SpaceShipPosition* spaceShipPos = nullptr;
	SpaceShip* newSpaceShip = nullptr;

	while (circularBuffer->getIfPacketsLeftToRead())
	{
		int packetId = circularBuffer->peekPacketId();

		std::cout << "Packet id: " << std::to_string(packetId) << std::endl;

		idProtocol* protocol = nullptr;
		testPosition* tst = nullptr;
		ComponentData* compData = nullptr;
		SpawnComponent* spawnComp = nullptr;
		PositionRotation* prMatrixData = nullptr;

		switch (packetId)
		{
		default:
			break;
		case PacketType::PACKETID:

			protocol = circularBuffer->readData<idProtocol>();
			std::cout << "received player id: " << std::to_string(protocol->assignedPlayerId) << std::endl;
			return protocol->assignedPlayerId;
			break;

		case PacketType::POSITIONROTATION:
			prMatrixData = circularBuffer->readData<PositionRotation>();

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


		case PacketType::SPAWNPLANETS:
			planetData = circularBuffer->readData<SpawnPlanets>();
			std::cout << "Received planet\n";
			planetVector.emplace_back(new Planet(meshes[0], DirectX::XMFLOAT3(planetData->size, planetData->size, planetData->size), DirectX::XMFLOAT3(planetData->xPos, planetData->yPos, planetData->zPos), (4.0f * 9.82f), meshes[1]));
			planetVector.back()->setPlanetShape(&physWorld);
			physWorld.setPlanets(planetVector);
			UwU++;
			break;

		case PacketType::SPACESHIPPOSITION:
			spaceShipPos = circularBuffer->readData<SpaceShipPosition>();
			
			//Create correct spaceship depending on team
			std::cout << "Spawned spaceship\n";
			if (spaceShips.size() == 0)
			{
				newSpaceShip = new SpaceShip(meshes[11], DirectX::SimpleMath::Vector3(spaceShipPos->x, spaceShipPos->y, spaceShipPos->z), 3, spaceShipPos->spaceShipTeam, field, meshes[9], DirectX::SimpleMath::Vector3(2, 2, 2), 4);
			}
			else
			{
				newSpaceShip = new SpaceShip(meshes[10], DirectX::SimpleMath::Vector3(spaceShipPos->x, spaceShipPos->y, spaceShipPos->z), 3, spaceShipPos->spaceShipTeam, field, meshes[9], DirectX::SimpleMath::Vector3(2, 2, 2), 4);
			}
			spaceShips.push_back(newSpaceShip);
			gameObjects.push_back(newSpaceShip);
			physWorld.addPhysComponent(newSpaceShip, reactphysics3d::CollisionShapeName::BOX, DirectX::XMFLOAT3(0.75f, 3 * 0.75f, 0.75f));
			newSpaceShip->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
			newSpaceShip->getPhysComp()->setRotation(DirectX::XMQuaternionRotationMatrix(newSpaceShip->getRot()));
			newSpaceShip->getPhysComp()->setPosition(reactphysics3d::Vector3(newSpaceShip->getPosV3().x, newSpaceShip->getPosV3().y, newSpaceShip->getPosV3().z));
			UwU++;
			break;
		}
	}
	return -1;
}

void PacketEventManager::TempLobbyHandleEvents(CircularBufferClient*& circularBuffer, const int& NROFPLAYERS, TempPlayerLobby playerLobby[], int& currentPlayerId, bool& startGame)
{

	while (circularBuffer->getIfPacketsLeftToRead())
	{
		int packetId = circularBuffer->peekPacketId(); 

		

		if (packetId == PacketType::PLAYERDATALOBBY)
		{
			PlayerData* playerData = circularBuffer->readData<PlayerData>();

			playerLobby[playerData->playerId].playerId = playerData->playerId;
			if (playerData->playerReady == 0)playerLobby[playerData->playerId].isReady = true;
			else playerLobby[playerData->playerId].isReady = false;

			playerLobby[playerData->playerId].isConnected = true;

			std::cout << "LOBBY recv playerData: " << playerData->playerId << std::endl;
		}
		else if (packetId == PacketType::PACKETID)
		{
			idProtocol* protocol = circularBuffer->readData<idProtocol>();
			std::cout << "LOBBY received player id: " << std::to_string(protocol->assignedPlayerId) << std::endl;
			currentPlayerId = protocol->assignedPlayerId;
			

		}
		else if (packetId == PacketType::LOBBYSTARTGAME)
		{
			LobbyStartGame* lbyStart = circularBuffer->readData<LobbyStartGame>();
			startGame = true;

			std::cout << "Recv start lobby start game\n";
		}
		else
		{
			//clear buffern ifall man får ett paket som inte var menat
			std::cout << "Clear Circlebuffer in Lobby\n";
			circularBuffer->clearBuffer();
		}
		
	}
}
