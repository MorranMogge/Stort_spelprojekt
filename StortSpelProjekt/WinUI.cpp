#include "stdafx.h"
#include "WinUI.h"
#include "Input.h"
#include "GPU.h"

void WinUI::HandleInputs()
{
	Input::Update();
	backText.IntersectMouse() ? backText.SetTint(DirectX::Colors::Green.v) : backText.SetTint(DirectX::Colors::White.v);

	if (Input::KeyPress(KeyCode::MOUSE_L))
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
	winText = GUISprite(1264.0f / 2.0f, 300);
	winText.Load(GPU::device, L"../Sprites/winText.png");

	backText = GUISprite(75 + 70, 600);
	backText.Load(GPU::device, L"../Sprites/backText.png");

	gameState = NOCHANGE;
}

WinUI::~WinUI()
{

}

void WinUI::Draw()
{
	GUI::Begin();
	HandleInputs();
	SpritePass();
	GUI::End();
}