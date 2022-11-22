#include "stdafx.h"
#include "MenuUI.h"
#include "GPU.h"
#include "Input.h"
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
	hit_control ? control2.Draw() : control.Draw();

	title.Draw();

}

void MenuUI::HandleInputs()
{
	Input::Update();
	auto state = gamePad->GetState(0);

	// any gamepad connected
	if (state.IsConnected())
	{
		// reset all to false
		hit_start = hit_setting = hit_credits = hit_exit = hit_control = false;

		static bool downTrigged = false;
		static bool upTrigged = false;

		// xbox r or l wheel down
		if (state.IsDPadDownPressed() || state.IsLeftThumbStickDown())
		{
			if (!downTrigged)
			{
				switch (selectIndex)
				{
				case 0:
					selectIndex = 1;
					break;
				case 1:
					selectIndex = 2;
					break;
				case 2:
					selectIndex = 3;
					break;
				case 3:
					selectIndex = 4;
					break;
				}
				downTrigged = true;
			}

		}
		else
		{
			downTrigged = false;
		}

		if (state.IsDPadUpPressed() || state.IsLeftThumbStickUp())
		{
			if (!upTrigged)
			{
				switch (selectIndex)
				{
				case 1:
					selectIndex = 0;
					break;
				case 2:
					selectIndex = 1;
					break;
				case 3:
					selectIndex = 2;
					break;
				case 4:
					selectIndex = 3;
					break;
				}
				upTrigged = true;
			}

		}
		else
		{
			upTrigged = false;
		}

		switch (selectIndex)
		{
		case 0:
			hit_start = true;
			break;
		case 1:
			hit_control = true;
			break;
		case 2:
			hit_setting = true;
			break;
		case 3:
			hit_credits = true;
			break;
		case 4:
			hit_exit = true;
			break;
		}
	}

	//else mouse
	else
	{
		hit_start = start.IntersectMouse();
		hit_setting = settings.IntersectMouse();
		hit_credits = credits.IntersectMouse();
		hit_exit = exit.IntersectMouse();
		hit_control = control.IntersectMouse();
	}



	if (Input::KeyPress(KeyCode::MOUSE_L) || state.IsAPressed())
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
		else if (hit_control)
		{
			gameState = CONTROL;
		}

	}

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

#define centerX 632
#define centerY 340
#define scale 0.3f,0.3f

	gamePad = std::make_unique<DirectX::GamePad>();
	start = GUISprite(centerX, 300);
	start.Load(L"../Sprites/Menu/start.png");
	start.SetScale(scale);

	control = GUISprite(centerX, 300 + 75 * 1);
	control.Load(L"../Sprites/Menu/control.png");
	control.SetScale(scale);

	settings = GUISprite(centerX, 300 + 75 * 2);
	settings.Load(L"../Sprites/Menu/settings.png");
	settings.SetScale(scale);

	credits = GUISprite(centerX, 300 + 75 * 3);
	credits.Load(L"../Sprites/Menu/credits.png");
	credits.SetScale(scale);

	exit = GUISprite(centerX, 300 + 75 * 4);
	exit.Load(L"../Sprites/Menu/exit.png");
	exit.SetScale(scale);

	start2 = GUISprite(centerX, 300);
	start2.Load(L"../Sprites/Menu/start2_r.png");
	start2.SetScale(scale);

	control2 = GUISprite(centerX, 300 + 75 * 1);
	control2.Load(L"../Sprites/Menu/control2_r.png");
	control2.SetScale(scale);

	settings2 = GUISprite(centerX, 300 + 75 * 2);
	settings2.Load(L"../Sprites/Menu/settings2_r.png");
	settings2.SetScale(scale);

	credits2 = GUISprite(centerX, 300 + 75 * 3);
	credits2.Load(L"../Sprites/Menu/credits2_r.png");
	credits2.SetScale(scale);

	exit2 = GUISprite(centerX, 300 + 75 * 4);
	exit2.Load(L"../Sprites/Menu/exit2_r.png");
	exit2.SetScale(scale);

	Loading = GUISprite(centerX, centerY);
	Loading.Load(L"../Sprites/Loading.bmp");
	Loading.SetScale(1, 1);

	title = GUISprite(centerX, 340-200);
	title.Load(L"../Sprites/title.png");
	title.SetScale(0.6f, 0.6f);

}

void MenuUI::Draw()
{
	HandleInputs();
	GUI::Begin();
	SpritePass();
	GUI::End();
}
