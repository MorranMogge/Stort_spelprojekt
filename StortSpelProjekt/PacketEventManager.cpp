#include "stdafx.h"
#include "PacketEventManager.h"

PacketEventManager::PacketEventManager()
{
}

void PacketEventManager::PacketHandleEvents(CircularBufferClient*& circularBuffer, const int &NROFPLAYERS, std::vector<Player*>& players, const int& playerId)
{
	//handles the online events
	while (circularBuffer->getIfPacketsLeftToRead())
	{
		int packetId = circularBuffer->peekPacketId();

		idProtocol* protocol = nullptr;
		testPosition* tst = nullptr;

		switch (packetId)
		{
		default:
			break;
		case PacketType::POSITION:

			protocol = circularBuffer->readData<idProtocol>();
			std::cout << "received player id: " << std::to_string(protocol->assignedPlayerId) << std::endl;
			break;

		case PacketType::PACKETID:

			tst = circularBuffer->readData<testPosition>();
			for (int i = 0; i < NROFPLAYERS; i++)
			{
				if (i == tst->playerId)
				{
					if (playerId != i)
					{
						players[i]->setPos(DirectX::XMFLOAT3(tst->x, tst->y, tst->z));
					}
				}
			}

			std::cout << "Checking return value from circular buffer player id.x: " << std::to_string(tst->playerId) << " y: " << std::to_string(tst->y) << std::endl;
			break;
		}

		//if (packetId == PacketType::POSITION)
		//{
		//	idProtocol* protocol = circularBuffer->readData<idProtocol>();
		//	std::cout << "received player id: " << std::to_string(protocol->assignedPlayerId) << std::endl;
		//}
		//else if (packetId == 10)
		//{
		//	testPosition* tst = circularBuffer->readData<testPosition>();
		//	for (int i = 0; i < NROFPLAYERS; i++)
		//	{
		//		if (i == tst->playerId)
		//		{
		//			if (playerId != i)
		//			{
		//				players[i]->setPos(DirectX::XMFLOAT3(tst->x, tst->y, tst->z));
		//			}
		//		}
		//	}

		//	std::cout << "Checking return value from circular buffer player id.x: " << std::to_string(tst->playerId) << " y: " << std::to_string(tst->y) << std::endl;
		//	//std::cout << "Player id: " << std::to_string(tst->playerId) << std::endl;
		//}
	}
}

int PacketEventManager::handleId(CircularBufferClient*& circularBuffer)
{
	while (circularBuffer->getIfPacketsLeftToRead())
	{
		int packetId = circularBuffer->peekPacketId();

		std::cout << "Packet id: " << std::to_string(packetId) << std::endl;

		idProtocol* protocol = nullptr;
		testPosition* tst = nullptr;

		switch (packetId)
		{
		default:
			break;
		case PacketType::POSITION:

			protocol = circularBuffer->readData<idProtocol>();
			std::cout << "received player id: " << std::to_string(protocol->assignedPlayerId) << std::endl;
			return protocol->assignedPlayerId;
			break;

		case PacketType::PACKETID:

			tst = circularBuffer->readData<testPosition>();

			std::cout << "throwing away position x: " << std::to_string(tst->playerId) << " y: " << std::to_string(tst->y) << std::endl;
			return -2;
			break;
		}

	}

	return -1;
}
