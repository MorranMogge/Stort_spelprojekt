#include "stdafx.h"
#include "LoseUI.h"
#include "Input.h"
#include "GPU.h"

void LoseUI::HandleInputs()
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

void LoseUI::SpritePass()
{
	loseText.Draw();
	backText.Draw();

}

GAMESTATE LoseUI::GetGameState()
{
	return gameState;
}

LoseUI::LoseUI()
{
	loseText = GUISprite(GPU::windowWidth / 2.0f, 300);
	loseText.Load(GPU::device, L"../Sprites/loseText.png");

	backText = GUISprite(75 + 70, 600);
	backText.Load(GPU::device, L"../Sprites/backText.png");

	gameState = NOCHANGE;
}

LoseUI::~LoseUI()
{

}

void LoseUI::Draw()
{
	GUI::Begin();
	HandleInputs();
	SpritePass();
	GUI::End();
}