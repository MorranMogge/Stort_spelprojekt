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

public:
	KingOfTheHillMiniGame(serverData& data);
	//Bestäm position på zonen
	void sendKingOfTheHillZone(serverData& data);
	//Check if players are inside the zone
	//SpawnBats
	//Check who won
	//update
};