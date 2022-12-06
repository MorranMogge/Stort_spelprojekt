#include "GetPlayerData.h"
#include "PacketEnum.h"

PlayerData getPlayerData(Player player)
{
    PlayerData playerData;

    playerData.packetId = PacketType::PLAYERDATA;
    playerData.playerId = player.getOnlineID();
    playerData.playerReady = player.getReadyStatus();
    

    return playerData;
}
