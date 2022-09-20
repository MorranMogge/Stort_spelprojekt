#include "CreditsMenu.h"

GAMESTATE CreditsMenu::Update()
{
	return ui.GetGameState();
}

void CreditsMenu::Render()
{
	ui.Draw();
}
