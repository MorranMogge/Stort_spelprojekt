#include "stdafx.h"
#include "InputIPUI.h"
#include "Input.h"

std::string GetClipboardText()
{
	OpenClipboard(nullptr);
	HANDLE hData = GetClipboardData(CF_TEXT);

	char* pszText = static_cast<char*>(GlobalLock(hData));
	std::string text(pszText);

	GlobalUnlock(hData);
	CloseClipboard();

	return text;
}

void InputIPUI::HandleInputs()
{
	if (Input::KeyPress(KeyCode::_0) || Input::KeyPress(KeyCode::Keypad0))
	{
		ip.append("0");
	}
	else if (Input::KeyPress(KeyCode::_1) || Input::KeyPress(KeyCode::Keypad1))
	{
		ip.append("1");
	}
	else if (Input::KeyPress(KeyCode::_2) || Input::KeyPress(KeyCode::Keypad2))
	{
		ip.append("2");
	}
	else if (Input::KeyPress(KeyCode::_3) || Input::KeyPress(KeyCode::Keypad3))
	{
		ip.append("3");
	}
	else if (Input::KeyPress(KeyCode::_4) || Input::KeyPress(KeyCode::Keypad4))
	{
		ip.append("4");
	}
	else if (Input::KeyPress(KeyCode::_5) || Input::KeyPress(KeyCode::Keypad5))
	{
		ip.append("5");
	}
	else if (Input::KeyPress(KeyCode::_6) || Input::KeyPress(KeyCode::Keypad6))
	{
		ip.append("6");
	}
	else if (Input::KeyPress(KeyCode::_7) || Input::KeyPress(KeyCode::Keypad7))
	{
		ip.append("7");
	}
	else if (Input::KeyPress(KeyCode::_8) || Input::KeyPress(KeyCode::Keypad8))
	{
		ip.append("8");
	}
	else if (Input::KeyPress(KeyCode::_9) || Input::KeyPress(KeyCode::Keypad9))
	{
		ip.append("9");
	}
	else if (Input::KeyPress(KeyCode::KeypadDecimal) || Input::KeyPress(KeyCode::Comma))
	{
		ip.append(".");
	}

	if (ip != "")
	{
		if (Input::KeyPress(KeyCode::Backspace))
		{
			ip.pop_back();
		}
	}
	if (Input::KeyDown(KeyCode::CTRL_L))
	{
		if (Input::KeyPress(KeyCode::V))
		{
			ip = GetClipboardText();
		}
	}

	hit_start = start.IntersectMouse();

	if (Input::KeyPress(KeyCode::MOUSE_L) && hit_start)
	{
		gameState = LOBBY;
	}

}

void InputIPUI::SpritePass()
{
	if (ip != "")
	{
		for (int i = 0; i < ip.size(); i++)
		{
#define offsetY 250 
#define centerX 632
#define centerY 340
#define textSpace 20.0f
#define posY1 centerY - 50
#define posX1 (centerX + (i * textSpace))-25
			switch (ip.at(i))
			{

			case '0':
			{
				n0.SetPosition({ posX1, posY1 });
				n0.Draw();
			}break;

			case '1':
			{
				n1.SetPosition({ posX1, posY1 });
				n1.Draw();
			}break;

			case '2':
			{
				n2.SetPosition({ posX1, posY1 });
				n2.Draw();
			}break;

			case '3':
			{
				n3.SetPosition({ posX1, posY1 });
				n3.Draw();
			}break;

			case '4':
			{
				n4.SetPosition({ posX1, posY1 });
				n4.Draw();
			}break;

			case '5':
			{
				n5.SetPosition({ posX1, posY1 });
				n5.Draw();
			}break;

			case '6':
			{
				n6.SetPosition({ posX1, posY1 });
				n6.Draw();
			}break;

			case '7':
			{
				n7.SetPosition({ posX1, posY1 });
				n7.Draw();
			}break;

			case '8':
			{
				n8.SetPosition({ posX1, posY1 });
				n8.Draw();
			}break;

			case '9':
			{
				n9.SetPosition({ posX1, posY1 });
				n9.Draw();
			}break;

			case '.':
			{
				punkt.SetPosition({ posX1, posY1 });
				punkt.Draw();
			}break;

			}

		}
	}

	ipText.Draw();
	hit_start ? start2.Draw() : start.Draw();
}

GAMESTATE InputIPUI::GetGameState()
{
	return gameState;
}

InputIPUI::InputIPUI()
{
#define nrScale 0.75f
	n0 = GUISprite(centerX + 200, centerY - 50);
	n0.Load(GPU::device, L"../Sprites/Numbers/0.png");
	n0.SetScale(nrScale, nrScale);

	n1 = GUISprite(centerX + 200, centerY - 50);
	n1.Load(GPU::device, L"../Sprites/Numbers/1.png");
	n1.SetScale(nrScale, nrScale);

	n2 = GUISprite(centerX + 200, centerY - 50);
	n2.Load(GPU::device, L"../Sprites/Numbers/2.png");
	n2.SetScale(nrScale, nrScale);

	n3 = GUISprite(centerX + 200, centerY - 50);
	n3.Load(GPU::device, L"../Sprites/Numbers/3.png");
	n3.SetScale(nrScale, nrScale);

	n4 = GUISprite(centerX + 200, centerY - 50);
	n4.Load(GPU::device, L"../Sprites/Numbers/4.png");
	n4.SetScale(nrScale, nrScale);

	n5 = GUISprite(centerX + 200, centerY - 50);
	n5.Load(GPU::device, L"../Sprites/Numbers/5.png");
	n5.SetScale(nrScale, nrScale);

	n6 = GUISprite(centerX + 200, centerY - 50);
	n6.Load(GPU::device, L"../Sprites/Numbers/6.png");
	n6.SetScale(nrScale, nrScale);

	n7 = GUISprite(centerX + 200, centerY - 50);
	n7.Load(GPU::device, L"../Sprites/Numbers/7.png");
	n7.SetScale(nrScale, nrScale);

	n8 = GUISprite(centerX + 200, centerY - 50);
	n8.Load(GPU::device, L"../Sprites/Numbers/8.png");
	n8.SetScale(nrScale, nrScale);

	n9 = GUISprite(centerX + 200, centerY - 50);
	n9.Load(GPU::device, L"../Sprites/Numbers/9.png");
	n9.SetScale(nrScale, nrScale);

	punkt = GUISprite(centerX + 200, centerY - 50);
	punkt.Load(GPU::device, L"../Sprites/punkt.png");
	punkt.SetScale(0.15f, 0.15f);

#define scale 0.3f,0.3f
#define scale2 0.3f,0.3f

	start = GUISprite(centerX, 300 + 200);
	start.Load(L"../Sprites/Menu/start.png");
	start.SetScale(scale);

	start2 = GUISprite(centerX, 300 + 200);
	start2.Load(L"../Sprites/Menu/start2_r.png");
	start2.SetScale(scale2);

	ipText = GUISprite(centerX - 75, centerY - 50);
	ipText.Load(L"../Sprites/ipText.png");
	ipText.SetScale(0.7f, 0.7f);

}

InputIPUI::~InputIPUI()
{

}

void InputIPUI::Draw()
{
	Input::Update();

	HandleInputs();
	GUI::Begin();

	SpritePass();

	GUI::End();
}
