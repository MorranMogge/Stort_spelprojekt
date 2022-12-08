#include "stdafx.h"
#include "LobbyUI.h"
#include "Lobby.h"
#include "Client.h"
//#include "ErrorLog.h"

Lobby::Lobby(Client*& client)
{
	//ErrorLog::Log("UWU");
	basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
	startGame = false;
	client = new Client("192.168.43.251");
	this->client = client;
	ifConnected = client->connectToServer();
	int currentPlayerOnlineId = -1;
	currentPlayerIsReady = -1;
}

Lobby::~Lobby()
{

}

GAMESTATE Lobby::Update()
{
	//returns to the menu if it doesnt connect to the server
	if (!ifConnected) return MENU;

	const int tmp = 2;

	this->packetEventManager.TempLobbyHandleEvents(this->client->getCircularBuffer(), tmp, this->playersLobby, currentPlayerOnlineId, startGame);

	if (ui.ready)
	{
		currentPlayerIsReady = 0;
	}
	else
	{
		currentPlayerIsReady = -1;
	}
	
	if (sendTimer.getTimePassed(0.5f))
	{
		PlayerData plData;

		plData.packetId = PacketType::PLAYERDATALOBBY;
		plData.playerId = currentPlayerOnlineId;
		plData.playerReady = currentPlayerIsReady;

		std::cout << "Sending PlayerReady: " << plData.playerReady << ", id: " << plData.playerId << std::endl;
		client->sendStuff<PlayerData>(plData);

		sendTimer.resetStartTime();
	}
	
	ui.p1_connect = playersLobby[0].isReady;
	ui.p2_connect = playersLobby[1].isReady;
	ui.p3_connect = playersLobby[2].isReady;
	ui.p4_connect = playersLobby[3].isReady;

	ui.p1 = playersLobby[0].isConnected;
	ui.p2 = playersLobby[1].isConnected;
	ui.p3 = playersLobby[2].isConnected;
	ui.p4 = playersLobby[3].isConnected;

	this->client->setClientId(currentPlayerOnlineId);

	//nr of connected players
	ui.connectedPlayers = 69;

	if (startGame)
	{
		
		ui.isLoading = true;// display loading screen
		return GAME;
	}


	return NOCHANGE;
}

void Lobby::Render()
{
	Input::Update();
	basicRenderer.setUpScene();
	ui.Draw();
}
