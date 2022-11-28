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

	n0 = GUISprite(centerX, 340 - 200);
	n0.Load(L"../Sprites/Numbers/0.png");
	n0.SetScale(0.6f, 0.6f);

	n1 = GUISprite(centerX, 340 - 200);
	n1.Load(L"../Sprites/Numbers/1.png");
	n1.SetScale(0.6f, 0.6f);

	n2 = GUISprite(centerX, 340 - 200);
	n2.Load(L"../Sprites/Numbers/2.png");
	n2.SetScale(0.6f, 0.6f);

	n3 = GUISprite(centerX, 340 - 200);
	n3.Load(L"../Sprites/Numbers/3.png");
	n3.SetScale(0.6f, 0.6f);

	n4 = GUISprite(centerX, 340 - 200);
	n4.Load(L"../Sprites/Numbers/4.png");
	n4.SetScale(0.6f, 0.6f);

	n5 = GUISprite(centerX, 340 - 200);
	n5.Load(L"../Sprites/Numbers/5.png");
	n5.SetScale(0.6f, 0.6f);

	n6 = GUISprite(centerX, 340 - 200);
	n6.Load(L"../Sprites/Numbers/6.png");
	n6.SetScale(0.6f, 0.6f);

	n7 = GUISprite(centerX, 340 - 200);
	n7.Load(L"../Sprites/Numbers/7.png");
	n7.SetScale(0.6f, 0.6f);

	n8 = GUISprite(centerX, 340 - 200);
	n8.Load(L"../Sprites/Numbers/8.png");
	n8.SetScale(0.6f, 0.6f);

	n9 = GUISprite(centerX, 340 - 200);
	n9.Load(L"../Sprites/Numbers/9.png");
	n9.SetScale(0.6f, 0.6f);
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

	std::string number = std::to_string((int)Time::CurrentTime());
	for (int i = 0; i < number.size(); i++)
	{
		#define textSpace 20.0f
		switch (number.at(i))
		{

		case '0':
		{
			n0.SetPosition({ 200.0f + (i * textSpace), 300.0f });
			n0.Draw();
		}break;

		case '1':
		{
			n1.SetPosition({ 200.0f + (i * textSpace), 300.0f });
			n1.Draw();
		}break;

		case '2':
		{
			n2.SetPosition({ 200.0f + (i * textSpace), 300.0f });
			n2.Draw();
		}break;

		case '3':
		{
			n3.SetPosition({ 200.0f + (i * textSpace), 300.0f });
			n3.Draw();
		}break;

		case '4':
		{
			n4.SetPosition({ 200.0f + (i * textSpace), 300.0f });
			n4.Draw();
		}break;

		case '5':
		{
			n5.SetPosition({ 200.0f + (i * textSpace), 300.0f });
			n5.Draw();
		}break;

		case '6':
		{
			n6.SetPosition({ 200.0f + (i * textSpace), 300.0f });
			n6.Draw();
		}break;

		case '7':
		{
			n7.SetPosition({ 200.0f + (i * textSpace), 300.0f });
			n7.Draw();
		}break;

		case '8':
		{
			n8.SetPosition({ 200.0f + (i * textSpace), 300.0f });
			n8.Draw();
		}break;

		case '9':
		{
			n9.SetPosition({ 200.0f + (i * textSpace), 300.0f });
			n9.Draw();
		}break;

		}

	}
}


void MiniGameUI::Draw()
{
	Input::Update();
	HandleInputs();
	GUI::Begin();
	SpritePass();
	GUI::End();
}
