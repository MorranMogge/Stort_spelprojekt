#include "stdafx.h"
#include "SettingsUI.h"
#include "Input.h"
#include "GuiHandler.h"

void SettingsUI::HandleInputs()
{

#pragma region backText

	if (backText.IntersectMouse())
	{
		backText.SetTint(DirectX::Colors::Green.v);
	}
	else
	{
		backText.SetTint(DirectX::Colors::White.v);
	}

	if (Input::KeyPress(KeyCode::MOUSE_L) && backText.GetTint() == DirectX::Colors::Green.v)
	{
		gameState = MENU;
	}

#pragma endregion



}

void SettingsUI::TextPass()
{

}

void SettingsUI::SpritePass()
{
	settingsText.Draw();
	backText.Draw();
	container.Draw();
}

GAMESTATE SettingsUI::GetGameState()
{
	return gameState;
}

SettingsUI::SettingsUI()
{

	settingsText = GUISprite(75 + 120, 100);
	settingsText.Load(GPU::device, L"../Sprites/settingText.png");

	backText = GUISprite(75 + 70, 600);
	backText.Load(GPU::device, L"../Sprites/backText.png");

	container = GUISprite(350 + 420, 75 + 300);
	container.Load(GPU::device, L"../Sprites/Container.png");

	gameState = NOCHANGE;
}

SettingsUI::~SettingsUI()
{

}

void SettingsUI::Draw()
{
	HandleInputs();
	GUI::Begin();
	SpritePass();
	TextPass();
	GUI::End();
}
