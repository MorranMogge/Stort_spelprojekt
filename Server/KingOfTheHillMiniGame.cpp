#include "KingOfTheHillMiniGame.h"

KingOfTheHillMiniGame::KingOfTheHillMiniGame(serverData& data, const short &nrOfPlayers)
	:kingOfTheHillOrigo(-50,0,0), radius(30), nrOfPlayers(nrOfPlayers)
{
	sendKingOfTheHillZone(data);
}

void KingOfTheHillMiniGame::sendKingOfTheHillZone(serverData& data)
{
	CreateZone zone;
	zone.xPos = kingOfTheHillOrigo.x;
	zone.yPos = kingOfTheHillOrigo.y;
	zone.zPos = kingOfTheHillOrigo.z;
	zone.scale = radius;
	zone.packetId = PacketType::CREATEZONE;
	sendBinaryDataAllPlayers<CreateZone>(zone, data);
}

void KingOfTheHillMiniGame::update(serverData& data)
{
	static float xPos;
	static float yPos;
	static float zPos;
	static DirectX::XMFLOAT3 playerPos;
	for(int i = 0; i < this->nrOfPlayers; i++)
	{
		xPos = data.users[i].playa.getposition('x');
		yPos = data.users[i].playa.getposition('y');
		zPos = data.users[i].playa.getposition('z');
		playerPos = DirectX::XMFLOAT3(xPos, yPos, zPos);
		subtractionXMFLOAT3(playerPos, kingOfTheHillOrigo);
		if (getLength(playerPos) <= radius)
		{
			std::cout << "Innanför zonen\n";
		}
		else
		{
			std::cout << "utanför zonen\n";
		}
	}
}