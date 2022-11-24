#include "PhysicsWorld.h"
#include "ServerPlanet.h"

#include <iostream>
#include <string>
#include <mutex>

#include <SFML/Network.hpp>
#include <chrono>
#include <thread>
#include <vector>
#include "player.h"
#include "Packethandler.h"
#include "CircularBuffer.h"
#include "Component.h"
#include "SpawnComponent.h"
#include "RandomizeSpawn.h"
#include "DirectXMathHelper.h"
#include "TimeStruct.h"

#include "KingOfTheHillMiniGame.h"

#include <d3d11_4.h>
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")

#include <psapi.h>

std::mutex mutex;

struct threadInfo
{
	userData* user;
	float pos[3];
	bool ifUserRecv;
	CircularBuffer* circBuffer;
};

bool receiveDataUdp(sf::Packet& receivedPacket, serverData &data, unsigned short& packetIdentifier)
{
	//remote adress
	sf::IpAddress remoteAddress;
	

	if (data.socket.receive(receivedPacket, remoteAddress, data.port) == sf::Socket::Done)
	{
		std::string receivedString;
		std::cout << "UDP received data from address: " << remoteAddress.toString() << std::endl;

		return true;
	}
	else
	{
		std::cout << "UDP failure\n";
		return false;
	}
};

void setupTcp(serverData& data)
{
	unsigned short listenport = 2001;
	data.tcpListener.listen(listenport);
	std::cout << "TCP listening on port: " << std::to_string((int)listenport) << std::endl;
};

void acceptPlayers(serverData& data)
{
	std::cout << "Accepting players TCP socket\n";

	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{

		if (data.tcpListener.accept(data.users[i].tcpSocket) == sf::TcpListener::Done)
		{
			std::cout << "TCP Accepted new player ipAdress: " << data.users[i].tcpSocket.getRemoteAddress().toString() << std::endl;
			data.users[i].ipAdress = data.users[i].tcpSocket.getRemoteAddress().toString();
			data.users[i].userName = "fixa username " + std::to_string(i + 1);
			data.users[i].playerId = i;
		}
		
	}
};

void sendDataAllPlayers(testPosition &posData, serverData& serverData)
{
	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		std::size_t recvSize;
		if (serverData.users[i].tcpSocket.send(&posData, sizeof(testPosition), recvSize) != sf::Socket::Done)
		{
			//error
			std::cout << "Couldnt send data to currentPlayer from array slot: " << std::to_string(i) << std::endl;
		}
		else
		{
			//std::cout << "sent data to currentPlayer: " << serverData.users[i].tcpSocket.getRemoteAddress().toString() << std::endl;
		}
	}
};

int extractBinaryPacketId(char* pointerToData[])
{
	int idPacket;
	memcpy(&idPacket, pointerToData, sizeof(int));
	
	return idPacket;
}

void recvData(void* param, userData* user)//thread to recv data
{
	threadInfo* data = (threadInfo*)param;

	std::cout << "ip from socket in thread: " << user->tcpSocket.getRemoteAddress().toString() << std::endl;
	while (1)
	{
		std::size_t recv;
		char datapointer[256];

		if (user->tcpSocket.receive(&datapointer, 256, recv) != sf::Socket::Done)
		{
			//error
		}
		else
		{
			mutex.lock();
			data->circBuffer->addData(datapointer, recv);
			mutex.unlock();
			
		}
		
	}
};

void sendIdToAllPlayers(serverData& data)
{
	int packetid = 10;

	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		idProtocol protocol;
		protocol.packetId = PacketType::PACKETID;
		protocol.assignedPlayerId = data.users[i].playerId;


		std::size_t sentSize;

		if (data.users[i].tcpSocket.send(&protocol, sizeof(protocol), sentSize) != sf::Socket::Done)
		{
			//error
			std::cout << "sendIdToAllPlayers(), couldnt send id to currentPlayer: " << std::to_string(i) << std::endl;
		}
		else
		{
			std::cout << "sendIdToAllPlayers() sent to currentPlayer nr: " << std::to_string(data.users[i].playerId) << std::endl;
		}
	}
};

int main()
{
	float flyTime = 0.f;
	float landingPoints[MAXNUMBEROFPLAYERS]{ 0.f };
	bool timeToFly = false;
	int progress[2] = { 0, 0 };
	std::vector<int> playersSent;
	int requests = 0;
	int itemid = 0;
	int componentIdCounter = 0;
	bool once = false;
	TimeStruct physicsTimer;
	PhysicsWorld physWorld;
	Component planetComp;
	MiniGames currentMinigame = MiniGames::COMPONENTCOLLECTION;
	physWorld.addPhysComponent(planetComp, reactphysics3d::CollisionShapeName::SPHERE, DirectX::XMFLOAT3(40, 40, 40));
	planetComp.getPhysicsComponent()->setType(reactphysics3d::BodyType::STATIC);

	std::string identifier;
	std::string s = "empty";
	// Group the variables to send into a packet

	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		landingPoints[i] = 0.f;
	}

	std::vector<DirectX::XMFLOAT3> spaceShipPos;
	spaceShipPos.emplace_back(DirectX::XMFLOAT3(-7.81178f, -37.8586f, -8.50119f));
	spaceShipPos.emplace_back(DirectX::XMFLOAT3(13.5817f, 35.9383f, 9.91351f));

	std::cout << "Nr of players for the game: " << std::to_string(MAXNUMBEROFPLAYERS) << std::endl;
	KingOfTheHillMiniGame miniGameKTH(MAXNUMBEROFPLAYERS);

	//std::vector<player> players;
	std::vector<Component> components;
	std::vector<Component> items;

	//ANVÄND DENNA FLR ALLA ITEMS OCH COMPONENTS
	std::vector<Item*> onlineItems;

	//sf::UdpSocket socket;
	std::string connectionType, mode;

	serverData data;
	sf::IpAddress remoteAddress;

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();

	data.port = 2001;

	std::cout << ip.toString() << "\n" << "port: " << data.port << std::endl;
	//serverData* serverData;
	//std::cout << "Starting handleReceiveData thread!\n";
	//std::thread* serverThread = new std::thread(handleReceivedData, &serverData);

	if (data.socket.bind(data.port) != sf::Socket::Done)
	{
		std::cout << "UDP Failed to bind UDP servern\n";
		return -1;
	}
	else
	{
		std::cout << "UDP Successfully bound socket\n";
	}
	
	std::chrono::time_point<std::chrono::system_clock> start, startComponentTimer, itemSpawnTimer;
	start = std::chrono::system_clock::now();

	float timerLength = 1.f / 30.0f;
	float timerComponentLength = 10.0f;
	float itemSpawnTimerLength = 20.0f;

	setupTcp(data);
	acceptPlayers(data);
	sendIdToAllPlayers(data);

	//Wait 3 seconds since we can lose some data if we directly send information about space ships
	physicsTimer.resetStartTime();
	while (!physicsTimer.getTimePassed(7.0f)) continue;

	//Spawning planets
	srand(time(0));
	std::vector<Planet*> planetVector;
	float planetSize = 40.f;
	int nrPlanets = 3;
	planetVector.emplace_back(new Planet(DirectX::XMFLOAT3(planetSize, planetSize, planetSize), DirectX::XMFLOAT3(0.f, 0.f, 0.f)));
	planetVector.back()->setPlanetShape(&physWorld);
	planetVector.emplace_back(new Planet(DirectX::XMFLOAT3(planetSize * 0.8f, planetSize * 0.8f, planetSize * 0.8f), DirectX::XMFLOAT3(55.f, 55.f, 55.f)));
	planetVector.back()->setPlanetShape(&physWorld);
	planetVector.emplace_back(new Planet(DirectX::XMFLOAT3(planetSize * 1.2f, planetSize * 1.2f, planetSize * 1.2f), DirectX::XMFLOAT3(-65.f, -65.f, 65.f)));
	planetVector.back()->setPlanetShape(&physWorld);
	physWorld.setPlanets(planetVector);

	for (int i = 0; i < planetVector.size(); i++)
	{
		SpawnPlanets planetData;
		planetData.packetId = PacketType::SPAWNPLANETS;
		planetData.xPos = planetVector[i]->getPlanetPosition().x;
		planetData.yPos = planetVector[i]->getPlanetPosition().y;
		planetData.zPos = planetVector[i]->getPlanetPosition().z;
		planetData.size = planetVector[i]->getSize();
		sendBinaryDataAllPlayers<SpawnPlanets>(planetData, data);
		std::cout << "Sent a planet\n";
	}

	//Sends information about the space ships to the clients
	for (int i = 0; i < spaceShipPos.size(); i++)
	{
		SpaceShipPosition spaceShipData;
		spaceShipData.packetId = PacketType::SPACESHIPPOSITION;
		spaceShipData.spaceShipTeam = i;
		spaceShipData.x = spaceShipPos[i].x;
		spaceShipData.y = spaceShipPos[i].y;
		spaceShipData.z = spaceShipPos[i].z;
		sendBinaryDataAllPlayers<SpaceShipPosition>(spaceShipData, data);
		std::cout << "Yes\n";
	}

	CircularBuffer* circBuffer = new CircularBuffer();
	std::thread* recvThread[MAXNUMBEROFPLAYERS];
	threadInfo threadData[MAXNUMBEROFPLAYERS];
	int offset = 25;
	for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
	{
		PhysicsComponent* newComp = new PhysicsComponent();
		physWorld.addPhysComponent(newComp);
		data.users[i].playa.setPhysicsComponent(newComp);
		newComp->setType(reactphysics3d::BodyType::KINEMATIC);
		threadData[i].pos[0] = 102.0f + (offset*i);
		threadData[i].pos[1] = 12.0f;
		threadData[i].pos[2] = -22.0f;
		threadData[i].circBuffer = circBuffer;
		recvThread[i] = new std::thread(recvData, &threadData[i], &data.users[i]);
	}

	//Starting timer
	start = std::chrono::system_clock::now();
	startComponentTimer = std::chrono::system_clock::now();
	itemSpawnTimer = std::chrono::system_clock::now();

	std::cout << "Starting while loop! \n";
	while (true)
	{

		while (circBuffer->getIfPacketsLeftToRead())
		{
			int packetId = circBuffer->peekPacketId();
			
			testPosition* tst = nullptr;
			ComponentData* compData = nullptr;
			PositionRotation* prMatrixData = nullptr;
			PlayerHit* playerHit = nullptr;
			itemPosition* itemPos = nullptr;
			ComponentDropped* cmpDropped = nullptr;
			ComponentRequestingPickUp* requestingCmpPickedUp = nullptr;
			LandingMiniSendScoreToServer* scoreFromClient = nullptr;
			MinigameStart* startMinigame = nullptr;
			DoneWithGame* requestStart = nullptr;

			switch (packetId)
			{
			default:
				break;

			case PacketType::POSITIONROTATION:
				prMatrixData = circBuffer->readData<PositionRotation>();
				//std::cout << "packet id: " << std::to_string(prMatrixData->playerId) << std::endl;
				for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
				{
					//std::cout << "playerid prMatrixData" << std::to_string(prMatrixData->) << std::endl;
					if (i == prMatrixData->playerId)
					{
						
						if (!data.users[i].playa.getDeathState())data.users[i].playa.setMatrix(prMatrixData->matrix);
						//std::cout <<"player Id: " << std::to_string(prMatrixData->playerId)<<"pos: " << std::to_string(data.users[i].playa.getMatrix()._14) << std::endl;
						break;
					}
				}
				break;

			case PacketType::POSITION:
				tst = circBuffer->readData<testPosition>();
				for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
				{
					if (i == tst->playerId)
					{
						if (!data.users[i].playa.getDeathState()) data.users[i].playa.setPosition(tst->x, tst->y, tst->z); else
						{
							reactphysics3d::Vector3 tempPos = data.users[i].playa.getPhysComp()->getPosition();
							data.users[i].playa.setPosition(tempPos.x, tempPos.y, tempPos.z);
						}
						break;
					}
				}
				break;

			case PacketType::COMPONENTPOSITION:
				compData = circBuffer->readData<ComponentData>();
				
				for (int i = 0; i < components.size(); i++)
				{
					if (components[i].getOnlineId() == compData->ComponentId)
					{
						//kanske inte GÖRA NÅGOT EFTERSOM MAN KAN TA POSITION FRÅN DEN SOM ÄR IN USE NÄR MAN FÅR SPELAR POSITION
					}
				}
				//use later
				
				break;

			case PacketType::COMPONENTDROPPED:
				cmpDropped = circBuffer->readData<ComponentDropped>();
				std::cout << "dropped package recv compId: " << std::to_string(cmpDropped->componentId) << std::endl;
				//find the component that is dropped
				for (int i = 0; i < components.size(); i++)
				{
					if (components[i].getOnlineId() == cmpDropped->componentId)
					{
						components[i].setInUseBy(-1);
						std::cout << std::to_string(components[i].getPosXMFLOAT3().x) << ", y: " << std::to_string(components[i].getPosXMFLOAT3().y) <<
							", z" << std::to_string(components[i].getPosXMFLOAT3().z) << std::endl;
					}
				}
				break;

			case PacketType::COMPONENTREQUESTINGPICKUP:
				requestingCmpPickedUp = circBuffer->readData<ComponentRequestingPickUp>();
				//std::cout << "cmprequestpickup comp id: " << std::to_string(requestingCmpPickedUp->componentId) << std::endl;
				std::cout << "components size: " << std::to_string(components.size()) << std::endl;
				for (int i = 0; i < components.size(); i++)
				{
					//kollar efter rätt component
					if (components[i].getOnlineId() == requestingCmpPickedUp->componentId)
					{
						//kollar om componenten inte används
						if (components[i].getInUseById() == -1)
						{
							std::cout << "picked up componentId: " << std::to_string(requestingCmpPickedUp->componentId) 
								<< ", by player: " << std::to_string(requestingCmpPickedUp->playerId) << std::endl;
							//skickar en bekräftelse till alla spelare att komponenten är upplockad av en spelare
							ConfirmComponentPickedUp sendConfirmComponentData;
							sendConfirmComponentData.playerPickUpId = requestingCmpPickedUp->playerId;
							sendConfirmComponentData.componentId = requestingCmpPickedUp->componentId;
							sendConfirmComponentData.packetId = PacketType::COMPONENTCONFIRMEDPICKUP;
							sendBinaryDataAllPlayers<ConfirmComponentPickedUp>(sendConfirmComponentData, data);
							components[i].setInUseBy(requestingCmpPickedUp->playerId);
						}
						else
						{
							std::cout << "A Player couldnt pick up component: " << std::to_string(requestingCmpPickedUp->componentId) << std::endl;
						}
					}
				}

				break;
			case PacketType::PLAYERHIT:
				playerHit = circBuffer->readData<PlayerHit>();
				sendBinaryDataOnePlayer<PlayerHit>(*playerHit, data.users[playerHit->playerId]);
				data.users[playerHit->playerId].playa.playerGotHit(reactphysics3d::Vector3(playerHit->xForce, playerHit->yForce, playerHit->zForce));
				break;

			case PacketType::ITEMPOSITION:
				itemPos = circBuffer->readData<itemPosition>();

				//for (int i = 0; i < items.size(); i++)
				//{
				//	//First check which component
				//	if (i == itemPos->itemId && items[i].getInUseById() != itemPos->inUseBy)
				//	{
				//		for (int j = 0; j < MAXNUMBEROFPLAYERS; j++)
				//		{
				//			if (itemPos->inUseBy == data.users[j].playerId && items[i].getInUseById() == -1)
				//			{
				//				items[i].setInUseBy(itemPos->inUseBy);
				//				//components[i].setPosition(compData->x, compData->y, compData->z);
				//				//components[i].setInUseBy(compData->inUseBy);
				//			}
				//			else if (itemPos->inUseBy == -1)
				//			{
				//				items[i].setInUseBy(-1);
				//				items[i].getPhysicsComponent()->setType(reactphysics3d::BodyType::STATIC);
				//				items[i].setPosition(compData->x, compData->y, compData->z);
				//				items[i].getPhysicsComponent()->setType(reactphysics3d::BodyType::DYNAMIC);
				//			}
				//		}
				//	}
				//}
				break;

			case PacketType::LANDINGMINIGAMESENDSCORETOSERVER:
				scoreFromClient = circBuffer->readData<LandingMiniSendScoreToServer>();
				landingPoints[scoreFromClient->playerId] = scoreFromClient->scoreToServer;
				break;

			case PacketType::DONEWITHGAME:
				requestStart = circBuffer->readData<DoneWithGame>();

				//Checking if the same player have already sent a request
				bool alreadySent = false;
				for (int i = 0; i < playersSent.size(); i++)
				{
					if (requestStart->playerID == playersSent[i]) alreadySent = true;
				}

				//It was not the same player
				if (!alreadySent)
				{
					playersSent.emplace_back(requestStart->playerID);
					requests++;

					//Starting new minigame if everyone is done
					if (requests == MAXNUMBEROFPLAYERS)
					{
						MinigameStart startGame;
						startGame.packetId = PacketType::STARTMINIGAMES;

						//Checks what the former game was
						if (requestStart->formerGame == MiniGames::INTERMISSION)
						{
							//Sending the capturezone
							miniGameKTH.sendKingOfTheHillZone(data);
							std::cout << "Sent capture zone\n";

							//Sending the planets
							planetVector[0]->setScale(DirectX::XMFLOAT3(60.f, 60.f, 60.f));
							planetVector[2]->setPosition(DirectX::XMFLOAT3(65.f, 65.f, 65.f));
							planetVector[2]->setScale(DirectX::XMFLOAT3(25.f, 25.f, 25.f));
							planetVector[1]->setPosition(DirectX::XMFLOAT3(-65.f, 65.f, 65.f));
							planetVector[1]->setScale(DirectX::XMFLOAT3(25.f, 25.f, 25.f));

							for (int i = 0; i < planetVector.size(); i++)
							{
								SpawnPlanets planetData;
								planetData.packetId = PacketType::SPAWNPLANETS;
								planetData.xPos = planetVector[i]->getPlanetPosition().x;
								planetData.yPos = planetVector[i]->getPlanetPosition().y;
								planetData.zPos = planetVector[i]->getPlanetPosition().z;
								planetData.size = planetVector[i]->getSize();
								sendBinaryDataAllPlayers<SpawnPlanets>(planetData, data);
								std::cout << "Sent a planet\n";
							}

							//Sending next minigame
							currentMinigame = MiniGames::LANDINGSPACESHIP;
							startGame.minigame = MiniGames::STARTLANDING;

						}
						else if (requestStart->formerGame == MiniGames::LANDINGSPACESHIP)
						{
							//Sending the next minigame
							currentMinigame = MiniGames::KINGOFTHEHILL;
							startGame.minigame = MiniGames::KINGOFTHEHILL;
						}
						sendBinaryDataAllPlayers<MinigameStart>(startGame, data);

						//Resetting
						requests = 0;
						playersSent.clear();
					}
				}
				break;
			}
		}

		//checks all components player position
		for (int i = 0; i < components.size(); i++)
		{
			for (int j = 0; j < MAXNUMBEROFPLAYERS; j++)
			{
				if (components[i].getInUseById() == data.users[j].playerId)
				{
					components[i].setPosition(data.users[j].playa.getposition('x'), data.users[j].playa.getposition('y'), data.users[j].playa.getposition('z'));
				}
			}
			//std::cout << "component in useBy: " << std::to_string(components[i].getInUseById()) << std::endl;
			
			//std::cout << "posX: " << std::to_string(components[i].getposition('x')) << "posY: " << std::to_string(components[i].getposition('y')) << std::endl;
		}

		//for (int i = 0; i < onlineItems.size(); i++)
		//{
		//	for (int j = 0; j < MAXNUMBEROFPLAYERS; j++)
		//	{
		//		if(onlineItems[i]->getInUseById() == data.users[j].playerId)
		//		{
		//			onlineItems[i]->setPosition(data.users[j].playa.getposition('x'), data.users[j].playa.getposition('y'), data.users[j].playa.getposition('z'));
		//		}
		//	}
		//}
		
		if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - startComponentTimer)).count() > timerComponentLength)
		{
			SpawnComponent cData = SpawnOneComponent(components, spaceShipPos);
			physWorld.addPhysComponent(components[components.size() - 1]);
			components[components.size() - 1].setPosition(cData.x, cData.y, cData.z);
			components[components.size() - 1].setOnlineId(componentIdCounter++);
			sendBinaryDataAllPlayers<SpawnComponent>(cData, data);
			startComponentTimer = std::chrono::system_clock::now();
		}


		//skickar itemSpawn
		if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - itemSpawnTimer)).count() > itemSpawnTimerLength)
		{
			//ItemSpawn itemSpawnData;
			//DirectX::XMFLOAT3 temp = randomizeObjectPos();
			//itemSpawnData.x = temp.x;
			//itemSpawnData.y = temp.y;
			//itemSpawnData.z = temp.z;
			//itemSpawnData.itemId = itemid++;
			//std::cout << "item spawn id: " << std::to_string(itemSpawnData.itemId) << std::endl;
			//itemSpawnData.packetId = PacketType::ITEMSPAWN;
			//itemSpawnData.inUseBy = -1;

			//items.push_back(Component());
			//physWorld.addPhysComponent(items[items.size() - 1]);
			//items[items.size() - 1].setPosition(temp.x, temp.y, temp.z);;
			//items[items.size() - 1].setInUseBy(-1);
			//sendBinaryDataAllPlayers(itemSpawnData, data);
			//itemSpawnTimer = std::chrono::system_clock::now();
		}
		
		
		//sends data based on the server tickrate
		if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count() > timerLength)
		{

			//WHAT NEEDS TO BE DONE ON SERVER TO HANDLE LANDING MINIGAME
			switch (currentMinigame)
			{
			case MiniGames::LANDINGSPACESHIP:
				//WE NEED TO RECIEVE POINTS, ADD THEM AND SEND THEM TO THE PLAYERS

				LandingMiniGameScore lScore;
				lScore.packetId = PacketType::LANDINGMINIGAMESCORE;
				int playerTeam;
				lScore.pointsBlueTeam = 0.f;
				lScore.pointsRedTeam = 0.f;

				for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
				{
					playerTeam = (MAXNUMBEROFPLAYERS) / 2;
					playerTeam = (int)(playerTeam < i + 1);
					if (playerTeam == 0) lScore.pointsRedTeam += landingPoints[i];
					else lScore.pointsBlueTeam += landingPoints[i];
				}

				sendBinaryDataAllPlayers<LandingMiniGameScore>(lScore, data);
				break;
			case MiniGames::KINGOFTHEHILL:
				miniGameKTH.update(data);
				break;

			/*default:
				break;*/
			}
			//if (currentMinigame == MiniGames::LANDINGSPACESHIP) continue; //We do not need to send more data if we are in landingMiniGame

			for (int i = 0; i < 10; i++)
			{
				physWorld.update(timerLength / 10.f);
			}

			//Check if any components are near after the physics update
			for (int i = 0; i < components.size(); i++)
			{
				for (int j = 0; j < spaceShipPos.size(); j++)
				{
					//kopiera det här för king of the hill
					//if (!components[i].getActiveState()) continue;
					static DirectX::XMFLOAT3 vecToComp;
					static DirectX::XMFLOAT3 objPos;
					vecToComp = spaceShipPos[j];
					objPos = components[i].getPhysicsComponent()->getPosV3();
					subtractionXMFLOAT3(vecToComp, objPos);
					if (getLength(vecToComp) <= 10.f)
					{
						//components[i].setInactive();
						std::cout << "getLength() <= 10.f\n";
						DirectX::XMFLOAT3 newCompPos = randomizeObjectPos();
						components[i].getPhysicsComponent()->setType(reactphysics3d::BodyType::STATIC);
						components[i].setPosition(newCompPos.x, newCompPos.y, newCompPos.z);
						components[i].getPhysicsComponent()->setType(reactphysics3d::BodyType::DYNAMIC);
						components[i].setInUseBy(-1);
						//KOM IHÅG SKICKA DATA TILL ALLA SPELARE FÖR ATT DEN ÄR  DROPPAD

						//Sending to client
						ComponentAdded compAdded;
						compAdded.packetId = PacketType::COMPONENTADDED;
						compAdded.spaceShipTeam = j;
						sendBinaryDataAllPlayers<ComponentAdded>(compAdded, data);

						//Checking if someone has won
						progress[j]++;
						if (progress[j] > 3)
						{
							progress[0] = 0;
							progress[1] = 0;
							timeToFly = true;
						}
					}
				}
			}

			//Waits for the ships to fly away before starting minigames
			if (timeToFly)
			{
				flyTime += timerLength;

				if (flyTime > 6.f)
				{
					std::cout << "SENT START MINIGAMES\n";
					MinigameStart startMinigame;
					startMinigame.packetId = PacketType::STARTMINIGAMES;
					startMinigame.minigame = MiniGames::STARTOFINTERMISSION;
					currentMinigame = MiniGames::INTERMISSION;

					if (progress[0] > 3)
					{
						startMinigame.pointsBlue = 100.f;
						startMinigame.pointsRed = 0.f;
					}
					else
					{
						startMinigame.pointsRed = 100.f;
						startMinigame.pointsBlue = 0.f;
					}
					sendBinaryDataAllPlayers<MinigameStart>(startMinigame, data);
					timeToFly = false;
				}
			}
			
			//physWorld.update(timerLength);
			//f�r varje spelare s� skicka deras position till alla klienter
			for (int i = 0; i < MAXNUMBEROFPLAYERS; i++)
			{
				//sendDataAllPlayers(pos, data);
				if (data.users[i].playa.getDeathState())
				{
					reactphysics3d::Vector3 tempPos = data.users[i].playa.getPhysComp()->getPosition();
					data.users[i].playa.setPosition(tempPos.x, tempPos.y, tempPos.z);
					data.users[i].playa.updatePosViaPhysComp();
				}

				PositionRotation prMatrix;
				prMatrix.ifDead = data.users[i].playa.getDeathState();
				prMatrix.matrix = data.users[i].playa.getMatrix();
				prMatrix.packetId = PacketType::POSITIONROTATION;
				prMatrix.playerId = i;
				
				sendBinaryDataAllPlayers(prMatrix, data);
			}

			
			////send component data to all players
			//for (int i = 0; i < components.size(); i++)
			//{
			//	ComponentData compData;
			//	
			//	compData.packetId = PacketType::COMPONENTPOSITION;
			//	compData.ComponentId = i;
			//	compData.inUseBy = components[i].getInUseById();
			//	compData.x = components[i].getposition('x');
			//	compData.y = components[i].getposition('y');
			//	compData.z = components[i].getposition('z');
			//	compData.quat = components[i].getPhysicsComponent()->getRotation();
			//	//if its in use by a player it will get the players position

			//	
			//	sendBinaryDataAllPlayers<ComponentData>(compData, data);
			//}

			for (int i = 0; i < components.size(); i++)
			{
				ComponentPosition compPosition;
				compPosition.ComponentId = components[i].getOnlineId();
				compPosition.packetId = PacketType::COMPONENTPOSITIONNEW;
				compPosition.x = components[i].getposition('x');
				compPosition.y = components[i].getposition('y');
				compPosition.z = components[i].getposition('z');
				//compPosition.quat = components[i].getPhysicsComponent()->getRotation();
				sendBinaryDataAllPlayers<ComponentPosition>(compPosition, data);

			}
			for (int i = 0; i < items.size(); i++)
			{
				itemPosition itemsPosData;
				itemsPosData.packetId = PacketType::ITEMPOSITION;
				itemsPosData.itemId = i;
				itemsPosData.inUseBy = items[i].getInUseById();
				itemsPosData.x = items[i].getposition('x');
				itemsPosData.y = items[i].getposition('y');
				itemsPosData.z = items[i].getposition('z');
				//sendBinaryDataAllPlayers<itemPosition>(itemsPosData, data);
			}

			start = std::chrono::system_clock::now();	
		}
	}

	for (int i = 0; i < planetVector.size(); i++)
	{
		delete planetVector[i];
	}
    return 0;

	//Hidden code
	/*if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - startComponentTimer)).count() > timerComponentLength && !once)
	{
		SpawnComponent cData = SpawnOneComponent(components);
		physWorld.addPhysComponent(components[components.size() - 1]);
		components[components.size() - 1].setPosition(cData.x, cData.y, cData.z);
		sendBinaryDataAllPlayers(cData, data);
		startComponentTimer = std::chrono::system_clock::now();
		once = true;
	}*/
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging 

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
