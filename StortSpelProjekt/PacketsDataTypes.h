#pragma once
#include <DirectXMath.h>
#include <reactphysics3d/reactphysics3d.h>

enum MiniGames
{
	COMPONENTCOLLECTION,
	LANDINGSPACESHIP,
	KINGOFTHEHILL,
	INTERMISSION,
	STARTOFINTERMISSION,
	STARTLANDING,
	DEFAULT
};

//enum = 3
struct PositionRotation
{
	int packetId;
	int playerId;
	DirectX::XMFLOAT4X4 matrix;
	int ifDead;
	int AnimId;
	float animSpeed;
	float xRot;
	float yRot;
	float zRot;
	float wRot;
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

	float RotX;
	float RotY;
	float RotZ;
};

//enum = 7
struct ItemSpawn
{
	int packetId;
	int itemId;
	int itemType;
	float x;
	float y;
	float z;
};

//enum = 8
struct ComponentDropped
{
	int packetId;
	int componentId;
	int playerId;
	float xPos;
	float yPos;
	float zPos;
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
	int componentID;
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
	float xRot;
	float yRot;
	float zRot;
	float wRot;
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

//enum = 24
struct LandingMiniGameScore
{
	int packetId;
	float pointsRedTeam;
	float pointsBlueTeam;
};

//enum = 25
struct LandingMiniGameOver
{
	int packetId;
	float totalPointsRedTeam;
	float totalPointsBlueTeam;
};

//enum = 26
struct LandingMiniSendScoreToServer
{
	int packetId;
	int playerId;
	float scoreToServer;
};

//enum 27
struct CreateZone
{
	int packetId;
	float xPos;
	float yPos;
	float zPos;
	float scale;
};

//enum = 28
struct MinigameStart
{
	int packetId;
	MiniGames minigame;
	float pointsBlue;
	float pointsRed;
};

//enum = 30
struct DoneWithGame
{
	int packetId;
	int playerID;
	MiniGames formerGame;
};

//enum 31
struct baseballBatSpawn
{
	int packetId;
	int baseBallBatId;
	int xPos;
	int yPos;
	int zPos;
};

//enum 32
struct PotionSpawn
{
	int packetId;
	int potionId;
	int xPos;
	int yPos;
	int zPos;
};

//enum = 69
struct UseGrenade
{
	int packetId;
	int itemId;
	float xForce;
	float yForce;
	float zForce;
};

//enum = 70
struct HitByGrenade
{
	int packetId;
	int itemId;
	int playerThatUsedTheItem;
	float xForce;
	float yForce;
	float zForce;
};

//enum = 71
struct UseBat
{
	int packetId;
	int itemId;
	int playerThatUsedTheItem;
	float xPos;
	float yPos;
	float zPos;
	float size;
};

//enum = 36
struct DoneLoading
{
	int packetId;
};