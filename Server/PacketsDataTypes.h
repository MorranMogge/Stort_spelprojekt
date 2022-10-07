#pragma once

struct testPosition
{
	int packetId;
	int playerId;
	float x;
	float y;
	float z;
};

//assign the player with an id that the server has created
struct idProtocol
{
	int packetId;
	int assignedPlayerId;
};
