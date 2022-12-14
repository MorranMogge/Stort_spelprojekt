#include "stdafx.h"
#include "MiniGameUI.h"
#include "Input.h"
#include "SoundLibrary.h"
#include "Time.h"
#include "Game.h"


MiniGameUI::MiniGameUI()
{
	using namespace DirectX::SimpleMath;
#define centerX 632
#define centerY 340

#define offsetY 250 
#define offsetX 50

#define scale 0.3f

	bar_b = GUISprite(centerX, 340 - offsetY);
	bar_b.Load(L"../Sprites/bar/background.png");
	bar_b.SetScale(scale, scale);

	bar_e = GUISprite(centerX, 340 - offsetY);
	bar_e.Load(L"../Sprites/bar/edge.png");
	bar_e.SetScale(scale, scale);

	bar_p = GUISprite(centerX - 105, 340 - offsetY - 12);
	bar_p.Load(L"../Sprites/bar/progress.png");
	bar_p.SetScale(50, scale);
	bar_p.SetOrigin({ 0,bar_p.GetHeight() * 0.5f });
	bar_p.SetTint(DirectX::Colors::Red.v);


	bar_b2 = GUISprite(centerX, 600 - offsetY);
	bar_b2.Load(L"../Sprites/bar/background.png");
	bar_b2.SetScale(scale, scale);

	bar_e2 = GUISprite(centerX, 600 - offsetY);
	bar_e2.Load(L"../Sprites/bar/edge.png");
	bar_e2.SetScale(scale, scale);

	bar_p2 = GUISprite(centerX - 105, 600 - offsetY - 12);
	bar_p2.Load(L"../Sprites/bar/progress.png");
	bar_p2.SetScale(50, scale);
	bar_p2.SetOrigin({ 0, bar_p2.GetHeight() * 0.5f });
	bar_p2.SetTint(DirectX::Colors::Blue.v);

#define scoreScale 0.35f
	scoreText = GUISprite(centerX, 340 - offsetY);
	scoreText.Load(L"../Sprites/ScoreText.png");
	scoreText.SetScale(scoreScale, scoreScale);

	scoreText2 = GUISprite(centerX, 600 - offsetY);
	scoreText2.Load(L"../Sprites/ScoreText.png");
	scoreText2.SetScale(scoreScale, scoreScale);

#define nrScale 0.4f
	n0 = GUISprite(centerX, 340 - offsetY);
	n0.Load(L"../Sprites/Numbers/0.png");
	n0.SetScale(nrScale, nrScale);

	n1 = GUISprite(centerX, 340 - offsetY);
	n1.Load(L"../Sprites/Numbers/1.png");
	n1.SetScale(nrScale, nrScale);

	n2 = GUISprite(centerX, 340 - offsetY);
	n2.Load(L"../Sprites/Numbers/2.png");
	n2.SetScale(nrScale, nrScale);

	n3 = GUISprite(centerX, 340 - offsetY);
	n3.Load(L"../Sprites/Numbers/3.png");
	n3.SetScale(nrScale, nrScale);

	n4 = GUISprite(centerX, 340 - offsetY);
	n4.Load(L"../Sprites/Numbers/4.png");
	n4.SetScale(nrScale, nrScale);

	n5 = GUISprite(centerX, 340 - offsetY);
	n5.Load(L"../Sprites/Numbers/5.png");
	n5.SetScale(nrScale, nrScale);

	n6 = GUISprite(centerX, 340 - offsetY);
	n6.Load(L"../Sprites/Numbers/6.png");
	n6.SetScale(nrScale, nrScale);

	n7 = GUISprite(centerX, 340 - offsetY);
	n7.Load(L"../Sprites/Numbers/7.png");
	n7.SetScale(nrScale, nrScale);

	n8 = GUISprite(centerX, 340 - offsetY);
	n8.Load(L"../Sprites/Numbers/8.png");
	n8.SetScale(nrScale, nrScale);

	n9 = GUISprite(centerX, 340 - offsetY);
	n9.Load(L"../Sprites/Numbers/9.png");
	n9.SetScale(nrScale, nrScale);

}

MiniGameUI::~MiniGameUI()
{
	
}

void MiniGameUI::setPointRed(const float point)
{
	pointRed = point;
}

void MiniGameUI::setPointBlue(const float point)
{
	pointBlue = point;
}

void MiniGameUI::HandleInputs()
{

	#define barvalue 53.0f
	bar_p.SetScale((float)Game::teamScores[0] / 115.0f * barvalue, scale);
	bar_p2.SetScale((float)Game::teamScores[1] / 115.0f * barvalue, scale);

}

void MiniGameUI::SpritePass()
{
	bar_b.Draw();
	bar_p.Draw();
	bar_e.Draw();

	bar_b2.Draw();
	bar_p2.Draw();
	bar_e2.Draw();

	//scoreText.Draw();
	//scoreText2.Draw();

	std::string number = std::to_string((int)Game::teamScores[0]);
	for (int i = 0; i < number.size(); i++)
	{
#define textSpace 12.0f
#define posY1 340 - offsetY
#define posX1 centerX + (i * textSpace)
		switch (number.at(i))
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

		}

	}

	std::string number2 = std::to_string((int)Game::teamScores[1]);
	for (int i = 0; i < number2.size(); i++)
	{
#define textSpace 12.0f
#define posY2 600 - offsetY
#define posX2 centerX + (i * textSpace)

		switch (number2.at(i))
		{

		case '0':
		{
			n0.SetPosition({ posX2, posY2 });
			n0.Draw();

		}break;

		case '1':
		{
			n1.SetPosition({ posX2, posY2 });
			n1.Draw();
		}break;

		case '2':
		{
			n2.SetPosition({ posX2, posY2 });
			n2.Draw();
		}break;

		case '3':
		{
			n3.SetPosition({ posX2, posY2 });
			n3.Draw();
		}break;

		case '4':
		{
			n4.SetPosition({ posX2, posY2 });
			n4.Draw();
		}break;

		case '5':
		{
			n5.SetPosition({ posX2, posY2 });
			n5.Draw();
		}break;

		case '6':
		{
			n6.SetPosition({ posX2, posY2 });
			n6.Draw();
		}break;

		case '7':
		{
			n7.SetPosition({ posX2, posY2 });
			n7.Draw();
		}break;

		case '8':
		{
			n8.SetPosition({ posX2, posY2 });
			n8.Draw();
		}break;

		case '9':
		{
			n9.SetPosition({ posX2, posY2 });
			n9.Draw();
		}break;

		}

	}
}


void MiniGameUI::Draw()
{
	HandleInputs();
	GUI::Begin();
	SpritePass();
	GUI::End();
}
