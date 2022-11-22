#include "stdafx.h"
#include "LoseUI.h"
#include "Input.h"
#include "GPU.h"

void LoseUI::HandleInputs()
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
	loseText = GUISprite(1264.0f / 2.0f, 300);
	loseText.Load(L"../Sprites/loseText.png");

	backText = GUISprite(75 + 70, 600);
	backText.Load(L"../Sprites/backText.png");

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