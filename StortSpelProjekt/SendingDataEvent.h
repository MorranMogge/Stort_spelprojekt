#pragma once
#include "Client.h"
#include "CircularBufferClient.h"
#include "Player.h"

void SendingDataEvent( Client*& client,  Player*& currentPlayer,  std::vector<Player*>& players);

void DroppedComponentSendingPacket(Client*& client);