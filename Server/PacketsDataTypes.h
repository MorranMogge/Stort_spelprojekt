#pragma once

//enum = 4
struct testPosition
{
	int packetId;
	int playerId;
	float x;
	float y;
	float z;
};

//assign the player with an id that the server has created
//enum = 10
struct idProtocol
{
	int packetId;
	int assignedPlayerId;
};
