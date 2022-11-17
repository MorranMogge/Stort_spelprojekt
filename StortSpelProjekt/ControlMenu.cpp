#include "stdafx.h"
#include "ControlMenu.h"

GAMESTATE ControlMenu::Update()
{
    return ui.GetGameState();
}

void ControlMenu::Render()
{
    basicRenderer.setUpScene();
    ui.Draw();
}

ControlMenu::ControlMenu()
{
    basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
}

ControlMenu::~ControlMenu()
{

}