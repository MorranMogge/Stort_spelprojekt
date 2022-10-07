#include "CreditsMenu.h"

GAMESTATE CreditsMenu::Update()
{
	return ui.GetGameState();
}

void CreditsMenu::Render()
{
	basicRenderer.setUpScene();
	ui.Draw();
}

CreditsMenu::CreditsMenu()
{
	basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
}

CreditsMenu::~CreditsMenu()
{

}