#pragma once
#include "State.h"
#include "BasicRenderer.h"
#include "LobbyUI.h"
#include "PacketEventManager.h"

class Lobby : public State
{
private:
	LobbyUI ui;
	BasicRenderer basicRenderer;
	PacketEventManager packetEventManager;
	Client* client;
	TempPlayerLobby playersLobby[4];
	int currentPlayerOnlineId;
	int currentPlayerIsReady;
	bool ifConnected;
	TimeStruct sendTimer;
	bool startGame;

public:

	Lobby(Client*& client);
	virtual ~Lobby() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};