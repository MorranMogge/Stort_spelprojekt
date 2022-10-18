#include "PacketEventManager.h"

PacketEventManager::PacketEventManager()
{
}

void PacketEventManager::PacketHandleEvents(CircularBuffer*& circularBuffer,const int &NROFPLAYERS, std::vector<Player*>& players, Client*& client)
{
	//handles the online events
	while (circularBuffer->getIfPacketsLeftToRead())
	{
		int packetId = circularBuffer->peekPacketId();

		if (packetId == 4)
		{
			idProtocol* protocol = circularBuffer->readData<idProtocol>();
			std::cout << "received player id: " << std::to_string(protocol->assignedPlayerId) << std::endl;
		}
		else if (packetId == 10)
		{
			testPosition* tst = circularBuffer->readData<testPosition>();
			for (int i = 0; i < NROFPLAYERS; i++)
			{
				if (i == tst->playerId)
				{
					if (client->getPlayerId() != i)
					{
						players[i]->setPos(DirectX::XMFLOAT3(tst->x, tst->y, tst->z));
					}
				}
			}

			std::cout << "Checking return value from circular buffer player id.x: " << std::to_string(tst->playerId) << " y: " << std::to_string(tst->y) << std::endl;
			//std::cout << "Player id: " << std::to_string(tst->playerId) << std::endl;
		}
	}
}
