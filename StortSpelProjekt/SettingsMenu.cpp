#include "SettingsMenu.h"

GAMESTATE SettingsMenu::Update()
{
    return ui.GetGameState();
}

void SettingsMenu::Render()
{
    ui.Draw();
}
