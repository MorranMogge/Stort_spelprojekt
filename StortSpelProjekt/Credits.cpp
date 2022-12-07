#include "stdafx.h"
#include "Credits.h"
#include "Input.h"
#include "SoundLibrary.h"

void Credits::HandleInputs()
{
	Input::Update();
	backText.IntersectMouse() ? backText.SetTint(DirectX::Colors::Green.v) : backText.SetTint(DirectX::Colors::White.v);

	if (Input::KeyPress(KeyCode::MOUSE_L))
	{
		if (backText.GetTint() == DirectX::Colors::Green.v)
		{
			SoundLibrary::clickSfx.stop();
			SoundLibrary::clickSfx.play();
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
	//creditsText.Draw();
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

	nameText = GUISprite(550, 340);
	nameText.Load(L"../Sprites/credits2.png");
	nameText.SetScale(0.6f, 0.6f);

	backText = GUISprite(75 + 70, 600);
	backText.Load(L"../Sprites/backText.png");

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
