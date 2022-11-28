#include "stdafx.h"
#include "MiniGameUI.h"
#include "Input.h"
#include "SoundLibrary.h"
#include "Time.h"


MiniGameUI::MiniGameUI()
{
#define centerX 632
#define centerY 340
	bar_b = GUISprite(centerX, 340 - 200);
	bar_b.Load(L"../Sprites/bar/background.png");
	bar_b.SetScale(0.6f, 0.6f);

	bar_e = GUISprite(centerX, 340 - 200);
	bar_e.Load(L"../Sprites/bar/edge.png");
	bar_e.SetScale(0.6f, 0.6f);

	bar_p = GUISprite(centerX - 210, 340 - 200 - 35);
	bar_p.Load(L"../Sprites/bar/progress.png");
	bar_p.SetScale(50, 0.6f);
	bar_p.SetOrigin({ 0,0 });
	bar_p.SetTint(DirectX::Colors::Blue.v);
}

MiniGameUI::~MiniGameUI()
{
	
}

void MiniGameUI::HandleInputs()
{
	bar_p.SetScale((sin(Time::CurrentTime()) * 0.5 + 0.5) * 107, 0.6f);
}

void MiniGameUI::SpritePass()
{
	bar_b.Draw();
	bar_p.Draw();
	bar_e.Draw();
}


void MiniGameUI::Draw()
{
	Input::Update();
	HandleInputs();
	GUI::Begin();
	SpritePass();
	GUI::End();
}
