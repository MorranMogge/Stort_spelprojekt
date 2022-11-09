#include "stdafx.h"
#include "MenuUI.h"
#include "GPU.h"
#include"Input.h"
#include "SettingsUI.h"
#include "Credits.h"

using namespace DirectX::SimpleMath;

void MenuUI::SpritePass()
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

void MenuUI::HandleInputs()
{
	Input::Update();
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
		else if (exit.GetTint() == DirectX::Colors::Green.v)
		{
			gameState = EXIT;
		}

	}

}

void MenuUI::TextPass()
{

}

GAMESTATE MenuUI::GetGameState()
{
	return gameState;
}

MenuUI::MenuUI()
{
	GUI::Init();

#define upp 60
#define left 80

	if (SettingsUI::is720p)
	{
		start = GUISprite(1000, 200);
		start.Load(GPU::device, L"../Sprites/start.png");
		start.SetScale(1, 1);

		settings = GUISprite(1000, 300);
		settings.Load(GPU::device, L"../Sprites/settings.png");
		settings.SetScale(1, 1);

		credits = GUISprite(1000, 400);
		credits.Load(GPU::device, L"../Sprites/credit.png");
		credits.SetScale(1, 1);

		exit = GUISprite(1000, 500);
		exit.Load(GPU::device, L"../Sprites/exit.png");
		exit.SetScale(1, 1);

		Loading = GUISprite(1264.0f / 2.0f, 681.0f / 2.0f);
		Loading.Load(GPU::device, L"../Sprites/Loading.bmp");
		Loading.SetScale(1, 1);

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
	}
	else
	{
		start = GUISprite(1000 / GUISprite::BaseWidth, 200 / GUISprite::BaseHeight);
		start.Load(GPU::device, L"../Sprites/start.png");
		start.SetScale(1 / GUISprite::BaseWidth, 1 / GUISprite::BaseHeight);

		settings = GUISprite(1000 / GUISprite::BaseWidth, 300 / GUISprite::BaseHeight);
		settings.Load(GPU::device, L"../Sprites/settings.png");
		settings.SetScale(1 / GUISprite::BaseWidth, 1 / GUISprite::BaseHeight);

		credits = GUISprite(1000 / GUISprite::BaseWidth, 400 / GUISprite::BaseHeight);
		credits.Load(GPU::device, L"../Sprites/credit.png");
		credits.SetScale(1 / GUISprite::BaseWidth, 1 / GUISprite::BaseHeight);

		exit = GUISprite(1000 / GUISprite::BaseWidth, 500 / GUISprite::BaseHeight);
		exit.Load(GPU::device, L"../Sprites/exit.png");
		exit.SetScale(1 / GUISprite::BaseWidth, 1 / GUISprite::BaseHeight);

		Loading = GUISprite((1264.0f / GUISprite::BaseWidth) / 2, (681.0f / 2.0f) / GUISprite::BaseHeight);
		Loading.Load(GPU::device, L"../Sprites/Loading.bmp");
		Loading.SetScale(1 / GUISprite::BaseWidth, 1 / GUISprite::BaseHeight);

		control = GUISprite((310 - left) / GUISprite::BaseWidth, (225 - upp) / GUISprite::BaseHeight);
		control.Load(GPU::device, L"../Sprites/control.png");
		control.SetScale(0.75 / GUISprite::BaseWidth, 0.75 / GUISprite::BaseHeight);

		useText = GUISprite((320 - left) / GUISprite::BaseWidth, (420 - upp) / GUISprite::BaseHeight);
		useText.Load(GPU::device, L"../Sprites/UseText.png");
		useText.SetScale(0.40f / GUISprite::BaseWidth, 0.40f / GUISprite::BaseHeight);

		throwText = GUISprite((340 - left) / GUISprite::BaseWidth, (500 - upp) / GUISprite::BaseHeight);
		throwText.Load(GPU::device, L"../Sprites/ThrowText.png");
		throwText.SetScale(0.40f / GUISprite::BaseWidth, 0.40f / GUISprite::BaseHeight);

		pickText = GUISprite((322 - left) / GUISprite::BaseWidth, (580 - upp) / GUISprite::BaseHeight);
		pickText.Load(GPU::device, L"../Sprites/PickText.png");
		pickText.SetScale(0.40f / GUISprite::BaseWidth, 0.40f / GUISprite::BaseHeight);

		objective = GUISprite((310 - left) / GUISprite::BaseWidth, (675 - upp) / GUISprite::BaseHeight);
		objective.Load(GPU::device, L"../Sprites/Objective.png");
		objective.SetScale(0.75 / GUISprite::BaseWidth, 0.75 / GUISprite::BaseHeight);
	}

	gameState = NOCHANGE;
}

void MenuUI::Draw()
{
	HandleInputs();
	GUI::Begin();
	SpritePass();
	TextPass();
	GUI::End();
}
