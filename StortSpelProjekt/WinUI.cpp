#include "stdafx.h"
#include "WinUI.h"
#include "Input.h"
#include "GPU.h"

void WinUI::HandleInputs()
{
	backText.IntersectMouse() ? backText.SetTint(DirectX::Colors::Green.v) : backText.SetTint(DirectX::Colors::White.v);
	auto state = gamePad->GetState(0);

	if (Input::KeyPress(KeyCode::MOUSE_L) || state.IsAPressed() || state.IsBPressed())
	{
		if (backText.GetTint() == DirectX::Colors::Green.v)
		{
			gameState = MENU;
		}
	}

}

void WinUI::SpritePass()
{
	winText.Draw();
	backText.Draw();

}

GAMESTATE WinUI::GetGameState()
{
	return gameState;
}

WinUI::WinUI()
{
	gamePad = std::make_unique<DirectX::GamePad>();

	winText = GUISprite((1264.0f / 2.0f), 300 - 100);
	winText.Load(L"../Sprites/victoryText.png");

	backText = GUISprite(75 + 70, 600);
	backText.Load(L"../Sprites/backText.png");

	gameState = NOCHANGE;
}

WinUI::~WinUI()
{

}

void WinUI::Draw()
{
	Input::Update();
	GUI::Begin();
	HandleInputs();
	SpritePass();
	GUI::End();
}