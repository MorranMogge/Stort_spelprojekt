#pragma once
#include"../Server/CircularBuffer.h"
#include"PacketEnum.h"
#include"Player.h"
#include"Client.h"

class PacketEventManager
{
private:

public:
	PacketEventManager();
	~PacketEventManager();

	void PacketHandleEvents(CircularBuffer*& circularBuffer, const int& NROFPLAYERS, std::vector<Player*>& players, Client*& client);
};