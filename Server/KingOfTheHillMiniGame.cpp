#include "KingOfTheHillMiniGame.h"

KingOfTheHillMiniGame::KingOfTheHillMiniGame(serverData& data)
	:kingOfTheHillOrigo(0,60,-40), radius(30)
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