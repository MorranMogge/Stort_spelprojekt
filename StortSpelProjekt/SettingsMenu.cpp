#include "SettingsMenu.h"

GAMESTATE SettingsMenu::Update()
{
    return ui.GetGameState();
}

void SettingsMenu::Render()
{
    basicRenderer.setUpScene();
    ui.Draw();
}

SettingsMenu::SettingsMenu()
{
    basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
}

SettingsMenu::~SettingsMenu()
{
    
}