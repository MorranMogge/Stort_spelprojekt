#include "stdafx.h"
#include "SettingsUI.h"
#include "Input.h"


void SettingsUI::HandleInputs()
{
	backText.IntersectMouse() ? backText.SetTint(DirectX::Colors::Green.v) : backText.SetTint(DirectX::Colors::White.v);

	if (Input::KeyPress(KeyCode::MOUSE_L))
	{
		if (backText.GetTint() == DirectX::Colors::Green.v)
		{
			gameState = MENU;
		}

		// click and intersects
		if (checkbox_true.IntersectMouse() && !dropdown2 && !dropdown1 || checkbox_false.IntersectMouse() && !dropdown2 && !dropdown1)
		{
			fullscreen = !fullscreen;
		}
		else if (dropDown.IntersectMouse())
		{
			dropdown1 = !dropdown1;
		}
		else if (dropDown2.IntersectMouse())
		{
			dropdown2 = !dropdown2;
		}

		else if (text720p2.IntersectMouse() && dropdown1)
		{
			is720p = true;
			dropdown1 = false;
			dropdown2 = false;
		}
		else if (text1080p2.IntersectMouse() && dropdown1)
		{
			is720p = false;
			dropdown1 = false;
			dropdown2 = false;
		}

		else if (directxText2.IntersectMouse() && dropdown2)
		{
			isDirectx = true;
			dropdown1 = false;
			dropdown2 = false;
		}
		else if (vulkanText2.IntersectMouse() && dropdown2)
		{
			isDirectx = false;
			dropdown1 = false;
			dropdown2 = false;
		}

		// reset dropdown
		else
		{
			dropdown1 = false;
			dropdown2 = false;
		}
	}

}

void SettingsUI::SpritePass()
{

	//draw stage
	settingsText.Draw();
	backText.Draw();

	dropDown.Draw();
	dropDown2.Draw();

	fullscreenText.Draw();
	graphicAPItext.Draw();
	resulotionText.Draw();

	fullscreen ? checkbox_true.Draw() : checkbox_false.Draw();

	is720p ? text720p.Draw() : text1080p.Draw();
	isDirectx ? directxText.Draw() : vulkanText.Draw();

	//if any dropdown is expanded
	if (dropdown1)
	{
		if (text1080p2.IntersectMouse())
		{
			dropDown_menu4.Draw();
		}
		else
		{
			dropDown_menu3.Draw();
		}

		text720p2.Draw();
		text1080p2.Draw();

	}
	if (dropdown2)
	{
		if (vulkanText2.IntersectMouse())
		{
			dropDown_menu2.Draw();
		}
		else
		{
			dropDown_menu1.Draw();
		}
		vulkanText2.Draw();
		directxText2.Draw();
	}

}

GAMESTATE SettingsUI::GetGameState()
{
	return gameState;
}

SettingsUI::SettingsUI()
{

	settingsText = GUISprite(75 + 120, 100);
	settingsText.Load(GPU::device, L"../Sprites/settingText.png");

	backText = GUISprite(75 + 70, 600);
	backText.Load(GPU::device, L"../Sprites/backText.png");

	#define scale { 0.75,0.75 }
	directxText = GUISprite(844, 406);
	directxText.Load(GPU::device, L"../Sprites/directxText.png");
	directxText.SetScale(scale);

	directxText2 = GUISprite(865, 460);
	directxText2.Load(GPU::device, L"../Sprites/directxText.png");
	directxText2.SetScale(scale);

	vulkanText = GUISprite(840, 406);
	vulkanText.Load(GPU::device, L"../Sprites/vulkanText.png");
	vulkanText.SetScale(scale);

	vulkanText2 = GUISprite(859, 510);
	vulkanText2.Load(GPU::device, L"../Sprites/vulkanText.png");
	vulkanText2.SetScale(scale);

	checkbox_false = GUISprite(726, 327);
	checkbox_false.Load(GPU::device, L"../Sprites/box_unchecked2.png");
	checkbox_false.SetScale(scale);

	checkbox_true = GUISprite(726, 327);
	checkbox_true.Load(GPU::device, L"../Sprites/box_checked2.png");
	checkbox_true.SetScale(scale);
	
	text720p = GUISprite(844, 229);
	text720p.Load(GPU::device, L"../Sprites/720pText.png");
	text720p.SetScale(scale);

	text720p2 = GUISprite(863, 281);
	text720p2.Load(GPU::device, L"../Sprites/720pText.png");
	text720p2.SetScale(scale);

	text1080p = GUISprite(844, 228);
	text1080p.Load(GPU::device, L"../Sprites/1080pText.png");
	text1080p.SetScale(scale);

	text1080p2 = GUISprite(863, 332);
	text1080p2.Load(GPU::device, L"../Sprites/1080pText.png");
	text1080p2.SetScale(scale);

	dropDown = GUISprite(862, 230);
	dropDown.Load(GPU::device, L"../Sprites/dropDown.png");
	dropDown.SetScale(scale);

	dropDown2 = GUISprite(862, 406);
	dropDown2.Load(GPU::device, L"../Sprites/dropDown.png");
	dropDown2.SetScale(scale);

	dropDown_menu1 = GUISprite(862, 485);
	dropDown_menu1.Load(GPU::device, L"../Sprites/dropDown_1.png");
	dropDown_menu1.SetScale(scale);

	dropDown_menu2 = GUISprite(862, 485);
	dropDown_menu2.Load(GPU::device, L"../Sprites/dropDown_2.png");
	dropDown_menu2.SetScale(scale);

	dropDown_menu3 = GUISprite(862, 309);
	dropDown_menu3.Load(GPU::device, L"../Sprites/dropDown_1.png");
	dropDown_menu3.SetScale(scale);

	dropDown_menu4 = GUISprite(862, 309);
	dropDown_menu4.Load(GPU::device, L"../Sprites/dropDown_2.png");
	dropDown_menu4.SetScale(scale);
	
	fullscreenText = GUISprite(325, 315);
	fullscreenText.Load(GPU::device, L"../Sprites/fullscreenText.png");
	fullscreenText.SetScale(scale);

	graphicAPItext = GUISprite(335, 75 + 325);
	graphicAPItext.Load(GPU::device, L"../Sprites/graphicAPI_text.png");
	graphicAPItext.SetScale(scale);

	resulotionText = GUISprite(330, 228);
	resulotionText.Load(GPU::device, L"../Sprites/resolutionText.png");
	resulotionText.SetScale(scale);
	

	gameState = NOCHANGE;
}

SettingsUI::~SettingsUI()
{

}

void SettingsUI::Draw()
{
	Input::Update();
	HandleInputs();
	GUI::Begin();
	SpritePass();
	GUI::End();

	//auto directxPos = directxText2.GetPosition();
	//imGui.spriteBegin();
	//imGui.spriteFloat("Directx X", directxPos.x, 0, 1280);
	//imGui.spriteFloat("Directx Y", directxPos.y, 0, 720);
	//imGui.spriteEnd();
	//directxText2.SetPosition(directxPos);

}
