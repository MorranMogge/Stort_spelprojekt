#include "stdafx.h"
#include "LobbyUI.h"

void LobbyUI::SpritePass()
{

}

LobbyUI::LobbyUI()
{
	GUI::Init();// ta väck senare eftersom det ska hända på ett annat ställe 
#define centerX 632
#define centerY 340
	Loading = GUISprite(centerX, centerY);
	Loading.Load(GPU::device, L"../Sprites/Loding.png");
	Loading.SetScale(1, 1);
}

LobbyUI::~LobbyUI()
{

}

void LobbyUI::Draw()
{
	GUI::Begin();

	Loading.Draw();

	GUI::End();
}
