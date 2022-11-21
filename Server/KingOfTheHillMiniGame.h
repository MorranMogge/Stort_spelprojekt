#pragma once
#define NOMINMAX

#include <d3d11.h>
#include "PacketEnum.h"
#include "PacketsDataTypes.h"
#include "../StortSpelProjekt/stdafx.h"

class KingOfTheHillMiniGame
{
private:
	
	//3D punkt
	DirectX::XMFLOAT3 kingOfTheHillOrigo;
	float radius;
public:
	KingOfTheHillMiniGame();
	//Bestäm position på zonen
	void sendKingOfTheHillZone();
	//Check if players are inside the zone
	//SpawnBats
	//Check who won
	//update
};