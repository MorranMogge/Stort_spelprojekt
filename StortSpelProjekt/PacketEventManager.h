#pragma once
#include "PacketEnum.h"
#include "Player.h"
#include "Component.h"
#include "PhysicsWorld.h"
#include "PacketsDataTypes.h"
#include "CircularBufferClient.h"
#include "SpaceShip.h"
#include "State.h"

struct TempPlayerLobby
{
	int playerId;
	bool isConnected = false;
	bool isReady;
};

class PacketEventManager
{
private:
	int planetCounter = 0;
	std::vector<bool> animated;

public:
	PacketEventManager();
	~PacketEventManager();

	void PacketHandleEvents(CircularBufferClient*& circularBuffer, const int& NROFPLAYERS, std::vector<Player*>& players, const int& playerId,
		std::vector<Component*>& compnentVector, PhysicsWorld& physWorld, std::vector<GameObject*>& gameObjects,
		GravityField* field, std::vector<SpaceShip*>& spaceShips, std::vector<Item*>& onlineItems, std::vector<Mesh*>& meshes, 
		std::vector<Planet*>& planetVector, CaptureZone*& captureZone, MiniGames& currentMinigame, float& redTeamPoints, float& blueTeamPoints, Client*& client, const float dt, GAMESTATE& currentGameState, DirectX::GamePad* gamePad, Mesh* planet1, Mesh* planet2, Mesh* planet3, std::vector<ID3D11ShaderResourceView*> srvArr, int teamScores[]);
		
	int handleId(CircularBufferClient*& circularBuffer, std::vector<Planet*>& planetVector, PhysicsWorld& physWorld, std::vector<Mesh*>& meshes,
		std::vector<SpaceShip*>& spaceShips, std::vector<GameObject*>& gameObjects, GravityField* field, int &UwU);

	void TempLobbyHandleEvents(CircularBufferClient*& circularBuffer, const int& NROFPLAYERS, TempPlayerLobby playerLobby[], int& currentPlayerId, bool& startGame);

};