#pragma once
#define NOMINMAX

#include <d3d11.h>
#include "PacketEnum.h"
#include "PacketsDataTypes.h"
#include "sendDataFunctions.h"



class KingOfTheHillMiniGame
{
private:
	
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
public:
	KingOfTheHillMiniGame(serverData& data, const short &nrOfPlayers);
	//Best�m position p� zonen
	void sendKingOfTheHillZone(serverData& data);
	void spawnItems();
	void update(serverData& data);
	//Check if players are inside the zone
	//SpawnBats
	//Check who won
	//update
};