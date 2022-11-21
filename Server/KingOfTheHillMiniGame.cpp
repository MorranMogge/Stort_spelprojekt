#include "KingOfTheHillMiniGame.h"

KingOfTheHillMiniGame::KingOfTheHillMiniGame()
	:kingOfTheHillOrigo(5,5,5), radius(10)
{

}

void KingOfTheHillMiniGame::sendKingOfTheHillZone()
{
	KingOfTheHillZone zone;
	zone.xPos = kingOfTheHillOrigo.x;
	zone.yPos = kingOfTheHillOrigo.y;
	zone.zPos = kingOfTheHillOrigo.z;
	zone.radius = radius;
	zone.packetId = PacketType::KINGOFTHEHILLZONE;
	//sendBinaryDataAllPlayers<KingOfTheHillZone>(zone, data);
}
