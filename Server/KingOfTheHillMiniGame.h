#pragma once
#define NOMINMAX
#include <vector>
#include <d3d11.h>
#include "PacketEnum.h"
#include "PacketsDataTypes.h"
#include "sendDataFunctions.h"



#include "BaseballBat.h"
#include "PhysicsWorld.h"
#include "RandomizeSpawn.h"


class KingOfTheHillMiniGame
{
private:
	std::chrono::time_point<std::chrono::system_clock> timer;
	std::chrono::time_point<std::chrono::system_clock> timerToSend;
	std::chrono::time_point<std::chrono::system_clock> itemSpawnTimer;
	float time;
	float timerSend;
	float timeToSpawnItems;
	bool redInside, blueInside, oneInside;

	//3D punkt
	DirectX::XMFLOAT3 kingOfTheHillOrigo;
	float radius;
	const short nrOfPlayers;
	void subtractionXMFLOAT3(DirectX::XMFLOAT3& argOne, const DirectX::XMFLOAT3& argTwo)
	{
		argOne.x -= argTwo.x;
		argOne.y -= argTwo.y;
		argOne.z -= argTwo.z;
	}
	float getLength(DirectX::XMFLOAT3 argOne)
	{
		return sqrt(argOne.x * argOne.x + argOne.y * argOne.y + argOne.z * argOne.z);
	}
	unsigned int pointsToAdd;
	unsigned int team1Score;
	unsigned int team2Score;
	unsigned int goalScore;
public:
	KingOfTheHillMiniGame(const short &nrOfPlayers);
	//Best�m position p� zonen
	void sendKingOfTheHillZone(serverData& data);
	void spawnItems();
	void update(serverData& data, std::vector<Item*>& onlineItems, PhysicsWorld& physWorld, int& componentIdCounter);

	//Check if players are inside the zone
	//SpawnBats
	//Check who won
	//update
};