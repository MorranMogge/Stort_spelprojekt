#include "stdafx.h"
#include "Menu.h"
#include "Input.h"
#include "Console.h"
#include "SoundCollection.h"
#include "GPU.h"


Menu::Menu()
{
	basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
}

Menu::~Menu()
{

}

GAMESTATE Menu::Update()
{
	return ui.GetGameState();
}

void Menu::Render()
{
	basicRenderer.setUpScene();
	ui.Draw();
}
