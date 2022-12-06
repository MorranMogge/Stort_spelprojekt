#include "stdafx.h"
#include "LobbyUI.h"
#include "Lobby.h"

Lobby::Lobby()
{
	basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);

}

Lobby::~Lobby()
{

}

GAMESTATE Lobby::Update()
{

	return NOCHANGE;
}

void Lobby::Render()
{
	basicRenderer.setUpScene();
	ui.Draw();
}
