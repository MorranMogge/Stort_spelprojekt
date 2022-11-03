#include "stdafx.h"
#include "GuiHandler.h"
#include "GPU.h"
#include"Input.h"
#include "SettingsUI.h"
#include "Credits.h"

using namespace DirectX::SimpleMath;

void GuiHandler::SpritePass()
{
	if (isLoading)
	{
		Loading.Draw();
		return;
	}

	start.Draw();
	settings.Draw();
	credits.Draw();
	exit.Draw();
	control.Draw();
	objective.Draw();
	useText.Draw();
	throwText.Draw();
	pickText.Draw();

}

void GuiHandler::HandleInputs()
{
	start.IntersectMouse() ? start.SetTint(DirectX::Colors::Green.v) : start.SetTint(DirectX::Colors::White.v);
	settings.IntersectMouse() ? settings.SetTint(DirectX::Colors::Green.v) : settings.SetTint(DirectX::Colors::White.v);
	credits.IntersectMouse() ? credits.SetTint(DirectX::Colors::Green.v) : credits.SetTint(DirectX::Colors::White.v);
	exit.IntersectMouse() ? exit.SetTint(DirectX::Colors::Green.v) : exit.SetTint(DirectX::Colors::White.v);

	if (Input::KeyPress(KeyCode::MOUSE_L))
	{
		if (start.GetTint() == DirectX::Colors::Green.v)
		{
			gameState = GAME;
			isLoading = true;
		}
		else if (settings.GetTint() == DirectX::Colors::Green.v)
		{
			gameState = SETTINGS;
		}
		else if (credits.GetTint() == DirectX::Colors::Green.v)
		{
			gameState = CREDITS;
		}
		else if (credits.GetTint() == DirectX::Colors::Green.v)
		{
			gameState = EXIT;
		}

	}

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

#define upp 60
#define left 80

	start = GUISprite(1000, 200);
	start.Load(GPU::device, L"../Sprites/start.png");

	settings = GUISprite(1000, 300);
	settings.Load(GPU::device, L"../Sprites/settings.png");

	credits = GUISprite(1000, 400);
	credits.Load(GPU::device, L"../Sprites/credit.png");

	exit = GUISprite(1000, 500);
	exit.Load(GPU::device, L"../Sprites/exit.png");

	Loading = GUISprite(1264.0f / 2.0f, 681.0f / 2.0f);
	Loading.Load(GPU::device, L"../Sprites/Loading.bmp");

	control = GUISprite(310 - left, 225 - upp);
	control.Load(GPU::device, L"../Sprites/control.png");
	control.SetScale(0.75, 0.75);
	
	useText = GUISprite(320 - left, 420 - upp);
	useText.Load(GPU::device, L"../Sprites/UseText.png");
	useText.SetScale(0.40f, 0.40f);

	throwText = GUISprite(340 - left, 500 - upp);
	throwText.Load(GPU::device, L"../Sprites/ThrowText.png");
	throwText.SetScale(0.40f, 0.40f);

	pickText = GUISprite(322 - left, 580 - upp);
	pickText.Load(GPU::device, L"../Sprites/PickText.png");
	pickText.SetScale(0.40f, 0.40f);

	objective = GUISprite(310 - left, 675 - upp);
	objective.Load(GPU::device, L"../Sprites/Objective.png");
	objective.SetScale(0.75, 0.75);

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
