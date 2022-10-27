#pragma once
#include <DirectXMath.h>

//enum = 3
struct PositionRotation
{
	int packetId;
	int playerId;
	DirectX::XMFLOAT4X4 matrix;
};

//enum = 4
struct testPosition
{
	int packetId;
	int playerId;
	float x;
	float y;
	float z;
};

//enum = 5
struct ComponentData
{
	int packetId;
	int ComponentId;
	int inUseBy; // if -1 its not in use
	float x;
	float y;
	float z;
};

//enum = 6
struct itemPosition
{
	int packetId;
	int itemId;
	int inUseBy;
	float x;
	float y;
	float z;
};

//enum = 7
struct ItemSpawn
{
	int packetId;
	int itemId;
	int inUseBy;
	float x;
	float y;
	float z;
};

//enum 8
struct ComponentDropped
{

};

//assign the player with an id that the server has created
//enum = 10
struct idProtocol
{
	int packetId;
	int assignedPlayerId;
};

//the winner gets this package
//enum = 11
struct winner
{
	int packetId;
};

//the loser gets this package
//enum = 12
struct Loser
{
	int packetId;
};

//enum = 13
struct SpawnComponent
{
	int packetId;
	int ComponentId;
	float x;
	float y;
	float z;
};