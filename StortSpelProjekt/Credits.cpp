#include "Credits.h"
#include "Input.h"
#include "GuiHandler.h"

void Credits::HandleInputs()
{
	if (show)
	{
#pragma region backText

		if (backText.IntersectMouse())
		{
			backText.SetTint(DirectX::Colors::Green.v);

			if (Input::KeyPress(KeyCode::MOUSE_L))
			{
				show = false;
				GuiHandler::show = true;
			}
		}
		else
		{
			backText.SetTint(DirectX::Colors::White.v);
		}

#pragma endregion
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

Credits::Credits()
{
	GUI::Init();

	creditsText = GUISprite(250 + 150, 150);
	creditsText.Load(GPU::device, L"../Sprites/CreditsText.png");
	creditsText.SetScale({ 0.7f, 0.7f });

	nameText = GUISprite(500 + 150, 300);
	nameText.Load(GPU::device, L"../Sprites/NameText.png");

	backText = GUISprite(75 + 70, 600);
	backText.Load(GPU::device, L"../Sprites/backText.png");

}

void Credits::Draw()
{
	HandleInputs();

	GUI::Begin();
	SpritePass();
	TextPass();
	GUI::End();
}
