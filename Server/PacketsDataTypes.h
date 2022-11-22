#pragma once
#include <DirectXMath.h>
#include <reactphysics3d/reactphysics3d.h>

//enum = 3
struct PositionRotation
{
	int packetId;
	int playerId;
	DirectX::XMFLOAT4X4 matrix;
	int ifDead;
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
struct ComponentData //ANV�NDS INTE
{
	int packetId;
	int ComponentId;
	int inUseBy; // if -1 its not in use
	float x;
	float y;
	float z;
	reactphysics3d::Quaternion quat;
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

//enum = 8
struct ComponentDropped
{
	int packetId;
	int componentId;
};

//enum = 9
struct ItemDropped
{
	int packetId;
	int componentId;
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

//enum = 14
struct SpaceShipPosition
{
	int packetId;
	int spaceShipTeam;
	float x;
	float y;
	float z;
};

//enum = 15
struct ComponentAdded
{
	int packetId;
	int spaceShipTeam;
};

//requesting to pick up component
//enum = {16}
struct ComponentRequestingPickUp
{
	int packetId;
	int componentId;
	int playerId;
};

//enum = 17
struct ItemPickedUp
{
	int packetId;
	int itemId;
};

//enum = 18
struct ConfirmComponentPickedUp
{
	int packetId;
	int componentId;
	int playerPickUpId;
};

//enum = 19
struct ComponentPosition
{
	int packetId;
	int ComponentId;
	float x;
	float y;
	float z;
	//reactphysics3d::Quaternion quat;
};

//enum = 21
struct PlayerHit
{
	int packetId;
	int playerId;
	float xForce;
	float yForce;
	float zForce;
};

//enum = 22
struct PlayerId
{
	int packetId;
	int playerId;
	int teamId;
};

//enum = 23
struct SpawnPlanets
{
	int packetId;
	float xPos;
	float yPos;
	float zPos;
	float size;
};

//enum 24
struct CreateZone
{
	int packetId;
	float xPos;
	float yPos;
	float zPos;
	float scale;
};

//enum = 25
struct IntermissionStart
{
	int packetId;
};

//enum = 25
//struct LandingMiniGameOver
//{
//	int packetId;
//	float points;
//};