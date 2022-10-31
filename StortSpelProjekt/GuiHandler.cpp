#include "stdafx.h"
#include "GuiHandler.h"
#include "GPU.h"
#include"Input.h"
#include "SettingsUI.h"
#include "Credits.h"

using namespace DirectX::SimpleMath;

void GuiHandler::SpritePass()
{
	start.Draw();
	settings.Draw();
	credits.Draw();
	exit.Draw();

	if (isLoading)
		Loading.Draw();
}

void GuiHandler::HandleInputs()
{

#pragma region start

	if (start.IntersectMouse())
	{
		start.SetTint(DirectX::Colors::Green.v);

		if (Input::KeyPress(KeyCode::MOUSE_L))
		{
			gameState = GAME;
			isLoading = true;
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
			gameState = SETTINGS;
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
			gameState = CREDITS;
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
			gameState = EXIT;
		}
	}
	else
	{
		exit.SetTint(DirectX::Colors::White.v);
	}

#pragma endregion

}

void GuiHandler::TextPass()
{

}

GAMESTATE GuiHandler::GetGameState()
{
	return gameState;
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

	Loading = GUISprite(GPU::windowWidth/ 2.0f, GPU::windowHeight / 2.0f);
	Loading.Load(GPU::device, L"../Sprites/Loading.bmp");

	gameState = NOCHANGE;
}

void GuiHandler::Draw()
{
	HandleInputs();
	GUI::Begin();
	SpritePass();
	TextPass();
	GUI::End();
}
