#include "stdafx.h"
#include "LoseMenu.h"

GAMESTATE LoseMenu::Update()
{
    return ui.GetGameState();
}

void LoseMenu::Render()
{
    basicRenderer.setUpScene();
    ui.Draw();
}

LoseMenu::LoseMenu()
{
    basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
}

LoseMenu::~LoseMenu()
{

}