#include "stdafx.h"
#include "MenuUI.h"
#include "GPU.h"
#include "Input.h"
#include "SettingsUI.h"
#include "Credits.h"
#include "SoundLibrary.h"
#include "Time.h"

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
		// reset all to false
		hit_start = hit_setting = hit_credits = hit_exit = hit_control = false;

		static bool downTrigged = false;
		static bool upTrigged = false;
		bool mouseIntersection = true;
		static int lastSelectIndex = 0;
		selectIndex = 6;

		if (start.IntersectMouse())
		{
			hit_start = true;
			lastSelectIndex = 0;
		}
		else if (control.IntersectMouse())
		{
			hit_control = true;
			lastSelectIndex = 1;
		}
		else if (settings.IntersectMouse())
		{
			hit_setting = true;
			lastSelectIndex = 2;
		}
		else if (credits.IntersectMouse())
		{
			hit_credits = true;
			lastSelectIndex = 3;
		}
		else if (exit.IntersectMouse())
		{
			hit_exit = true;
			lastSelectIndex = 4;
		}
		else
		{
			mouseIntersection = false;
		}


		if (!mouseIntersection)
		{
			// W or UP
			if (Input::KeyPress(KeyCode::S) || Input::KeyPress(KeyCode::ARROW_Down))
			{
				if (selectIndex == 6)
				{
					selectIndex = lastSelectIndex;
				}
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

			if (Input::KeyPress(KeyCode::W) || Input::KeyPress(KeyCode::ARROW_Up))
			{
				if (selectIndex == 6)
				{
					selectIndex = lastSelectIndex;
				}
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

			if (lastSelectIndex != selectIndex && selectIndex != 6)
			{
				//std::cout << "changed Indx: " << std::endl;
				lastSelectIndex = selectIndex;
			}

			//std::cout << "lastIndx: " << lastSelectIndex << std::endl;
			switch (lastSelectIndex)
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
		
	}




	if (Input::KeyPress(KeyCode::MOUSE_L) || state.IsAPressed() || 	(Input::KeyPress(KeyCode::ENTER)))
	{
		if (hit_start)
		{
			SoundLibrary::menuMusic.stop();
			SoundLibrary::clickSfx.stop();
			SoundLibrary::clickSfx.play();
			gameState = GAME;
			isLoading = true;
		}
		else if (hit_setting)
		{
			SoundLibrary::clickSfx.stop();
			SoundLibrary::clickSfx.play();
			gameState = SETTINGS;
		}
		else if (hit_credits)
		{
			SoundLibrary::clickSfx.stop();
			SoundLibrary::clickSfx.play();
			gameState = CREDITS;
		}
		else if (hit_exit)
		{
			SoundLibrary::menuMusic.stop();
			SoundLibrary::clickSfx.stop();
			SoundLibrary::clickSfx.play();
			gameState = EXIT;
		}
		else if (hit_control)
		{
			SoundLibrary::clickSfx.stop();
			SoundLibrary::clickSfx.play();
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
#define scale2 0.35f,0.35f

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
	start2.SetScale(scale2);

	control2 = GUISprite(centerX, 300 + 75 * 1);
	control2.Load(L"../Sprites/Menu/control2_r.png");
	control2.SetScale(scale2);

	settings2 = GUISprite(centerX, 300 + 75 * 2);
	settings2.Load(L"../Sprites/Menu/settings2_r.png");
	settings2.SetScale(scale2);

	credits2 = GUISprite(centerX, 300 + 75 * 3);
	credits2.Load(L"../Sprites/Menu/credits2_r.png");
	credits2.SetScale(scale2);

	exit2 = GUISprite(centerX, 300 + 75 * 4);
	exit2.Load(L"../Sprites/Menu/exit2_r.png");
	exit2.SetScale(scale2);

	Loading = GUISprite(centerX, centerY);
	Loading.Load(L"../Sprites/Loding.png");
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
