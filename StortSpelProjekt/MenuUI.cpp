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

	hit_start ? start2.Draw() : start.Draw();
	hit_setting? settings2.Draw() :settings.Draw();
	hit_credits ? credits2.Draw() : credits.Draw();
	hit_exit ? exit2.Draw() : exit.Draw();
	control.Draw();
	objective.Draw();
	useText.Draw();
	throwText.Draw();
	pickText.Draw();
	title.Draw();

}

void MenuUI::HandleInputs()
{
	Input::Update();

	hit_start = start.IntersectMouse();
	hit_setting = settings.IntersectMouse();
	hit_credits = credits.IntersectMouse();
	hit_exit = exit.IntersectMouse();

	if (Input::KeyPress(KeyCode::MOUSE_L))
	{
		if (hit_start)
		{
			gameState = GAME;
			isLoading = true;
		}
		else if (hit_setting)
		{
			gameState = SETTINGS;
		}
		else if (hit_credits)
		{
			gameState = CREDITS;
		}
		else if (hit_exit)
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

#define center 632
#define scale 0.3f,0.3f

	start = GUISprite(center, 300);
	start.Load(GPU::device, L"../Sprites/Menu/start.png");
	start.SetScale(scale);

	settings = GUISprite(center, 300 + 75);
	settings.Load(GPU::device, L"../Sprites/Menu/settings.png");
	settings.SetScale(scale);

	credits = GUISprite(center, 300 + 75 * 2);
	credits.Load(GPU::device, L"../Sprites/Menu/credits.png");
	credits.SetScale(scale);

	exit = GUISprite(center, 300 + 75 * 3);
	exit.Load(GPU::device, L"../Sprites/Menu/exit.png");
	exit.SetScale(scale);

	start2 = GUISprite(center, 300);
	start2.Load(GPU::device, L"../Sprites/Menu/start2_r.png");
	start2.SetScale(scale);

	settings2 = GUISprite(center, 300 + 75);
	settings2.Load(GPU::device, L"../Sprites/Menu/settings2_r.png");
	settings2.SetScale(scale);

	credits2 = GUISprite(center, 300 + 75 * 2);
	credits2.Load(GPU::device, L"../Sprites/Menu/credits2_r.png");
	credits2.SetScale(scale);

	exit2 = GUISprite(center, 300 + 75 * 3);
	exit2.Load(GPU::device, L"../Sprites/Menu/exit2_r.png");
	exit2.SetScale(scale);

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
	objective.SetScale(0.75f, 0.75f);

	title = GUISprite(center, 340-200);
	title.Load(GPU::device, L"../Sprites/title.png");
	title.SetScale(0.6f, 0.6f);

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
