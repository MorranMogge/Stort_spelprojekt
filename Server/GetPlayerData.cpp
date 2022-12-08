#include "GetPlayerData.h"
#include "PacketEnum.h"

PlayerData getPlayerData(const Player& player)
{
    PlayerData playerData;

    playerData.packetId = PacketType::PLAYERDATALOBBY;
    playerData.playerId = player.getOnlineID();
    playerData.playerReady = player.getReadyStatus();
    

    return playerData;
}
