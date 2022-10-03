#include "stdafx.h"
#include "GuiHandler.h"
#include "GPU.h"
#include"Input.h"
#include "SettingsUI.h"
#include "Credits.h"

void GuiHandler::SpritePass()
{
	start.Draw();
	settings.Draw();
	credits.Draw();
	exit.Draw();
}

void GuiHandler::HandleInputs()
{
	if (show)
	{
#pragma region start

		if (start.IntersectMouse())
		{
			start.SetTint(DirectX::Colors::Green.v);

			if (Input::KeyPress(KeyCode::MOUSE_L))
			{
				std::cout << "start\n";
			}
		}
		else
		{
			start.SetTint(DirectX::Colors::White.v);
		}

#pragma endregion

#pragma region settings

		if (settings.IntersectMouse())
		{
			settings.SetTint(DirectX::Colors::Green.v);

			if (Input::KeyPress(KeyCode::MOUSE_L))
			{
				show = false;
				SettingsUI::show = true;
			}
		}
		else
		{
			settings.SetTint(DirectX::Colors::White.v);
		}

#pragma endregion

#pragma region credits

		if (credits.IntersectMouse())
		{
			credits.SetTint(DirectX::Colors::Green.v);

			if (Input::KeyPress(KeyCode::MOUSE_L))
			{
				show = false;
				Credits::show = true;
			}
		}
		else
		{
			credits.SetTint(DirectX::Colors::White.v);
		}

#pragma endregion

#pragma region exit

		if (exit.IntersectMouse())
		{
			exit.SetTint(DirectX::Colors::Green.v);

			if (Input::KeyPress(KeyCode::MOUSE_L))
			{
				isExit = true;
			}
		}
		else
		{
			exit.SetTint(DirectX::Colors::White.v);
		}

#pragma endregion
	}



}

void GuiHandler::TextPass()
{
	testText.Draw();
}

GuiHandler::GuiHandler()
{
	GUI::Init();

	start = GUISprite(1000, 200);
	start.Load(GPU::device, L"../Sprites/start.png");

	settings = GUISprite(1000, 300);
	settings.Load(GPU::device, L"../Sprites/settings.png");

	credits = GUISprite(1000, 400);
	credits.Load(GPU::device, L"../Sprites/credit.png");

	exit = GUISprite(1000, 500);
	exit.Load(GPU::device, L"../Sprites/exit.png");

	testText = GUIText(500, 400);
	testText.Load(GPU::device, L"../Sprites/GothenbergTextura.spritefont");
	testText.setText(L"Test Text");

}

void GuiHandler::Draw()
{
	HandleInputs();
	GUI::Begin();
	SpritePass();
	TextPass();
	GUI::End();
}