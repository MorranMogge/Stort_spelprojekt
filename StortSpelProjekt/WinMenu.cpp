#include "stdafx.h"
#include "WinMenu.h"

GAMESTATE WinMenu::Update()
{
    return ui.GetGameState();
}

void WinMenu::Render()
{
    basicRenderer.setUpScene();
    ui.Draw();
}

WinMenu::WinMenu()
{
    basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
}

WinMenu::~WinMenu()
{

}