#include "stdafx.h"
#include "Credits.h"
#include "Input.h"

void Credits::HandleInputs()
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

	if (auto state = gamePad->GetState(0); state.IsConnected())
	{
		if (state.IsBPressed())
		{
			gameState = MENU;
		}
	}

}

void Credits::TextPass()
{

}

void Credits::SpritePass()
{
	creditsText.Draw();
	nameText.Draw();
	backText.Draw();
}


GAMESTATE Credits::GetGameState()
{
	return gameState;
}

Credits::Credits()
{
	gamePad = std::make_unique<DirectX::GamePad>();

	creditsText = GUISprite(250 + 150, 150);
	creditsText.Load(GPU::device, L"../Sprites/CreditsText.png");
	creditsText.SetScale({ 0.7f, 0.7f });

	nameText = GUISprite(500 + 150, 300);
	nameText.Load(GPU::device, L"../Sprites/NameText.png");

	backText = GUISprite(75 + 70, 600);
	backText.Load(GPU::device, L"../Sprites/backText.png");

	gameState = NOCHANGE;

}

Credits::~Credits()
{

}

void Credits::Draw()
{
	HandleInputs();

	GUI::Begin();
	SpritePass();
	TextPass();
	GUI::End();
}
