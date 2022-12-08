#include "stdafx.h"
#include "LoseUI.h"
#include "Input.h"
#include "GPU.h"

void LoseUI::HandleInputs()
{
	auto state = gamePad->GetState(0);
	backText.IntersectMouse() ? backText.SetTint(DirectX::Colors::Green.v) : backText.SetTint(DirectX::Colors::White.v);

	if (Input::KeyPress(KeyCode::MOUSE_L) || state.IsAPressed() || state.IsBPressed())
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
	gamePad = std::make_unique<DirectX::GamePad>();
	loseText = GUISprite((1264.0f / 2.0f), 300 - 100);
	loseText.Load(L"../Sprites/loseText2.png");

	backText = GUISprite(75 + 70, 600);
	backText.Load(L"../Sprites/backText.png");

	gameState = NOCHANGE;
}

LoseUI::~LoseUI()
{

}

void LoseUI::Draw()
{
	Input::Update();
	HandleInputs();
	GUI::Begin();
	SpritePass();
	GUI::End();
}