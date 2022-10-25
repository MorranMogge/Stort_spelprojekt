#include "stdafx.h"
#include "HudUI.h"
#include "Input.h"


void HudUI::SpritePass()
{
	if (SpaceShip::red)
	{
		switch (SpaceShip::red->getNrOfComponents())
		{
		case 0:
			redTeam0.Draw(); break;
		case 1:
			redTeam1.Draw(); break;
		case 2:
			redTeam2.Draw(); break;
		case 3:
			redTeam3.Draw(); break;
		case 4:
			redTeam4.Draw(); break;
		}
	}

	if (SpaceShip::blue)
	{
		switch (SpaceShip::blue->getNrOfComponents())
		{
		case 0:
			blueTeam0.Draw(); break;
		case 1:
			blueTeam1.Draw(); break;
		case 2:
			blueTeam2.Draw(); break;
		case 3:
			blueTeam3.Draw(); break;
		case 4:
			blueTeam4.Draw(); break;
		}
	}

}

HudUI::HudUI()
{
	using namespace DirectX::SimpleMath;

	#define Scale 0.4f

	#define PositionRed Vector2(50, 620)
	#define PositionBlue Vector2(125, 620)

	redTeam0 = GUISprite(PositionRed);
	redTeam0.Load(GPU::device, L"../Sprites/team_r_0.png");
	redTeam0.SetScale(Scale, Scale);

	redTeam1 = GUISprite(PositionRed);
	redTeam1.Load(GPU::device, L"../Sprites/team_r_1.png");
	redTeam1.SetScale(Scale, Scale);

	redTeam2 = GUISprite(PositionRed);
	redTeam2.Load(GPU::device, L"../Sprites/team_r_2.png");
	redTeam2.SetScale(Scale, Scale);

	redTeam3 = GUISprite(PositionRed);
	redTeam3.Load(GPU::device, L"../Sprites/team_r_3.png");
	redTeam3.SetScale(Scale, Scale);

	redTeam4 = GUISprite(PositionRed);
	redTeam4.Load(GPU::device, L"../Sprites/team_r_4.png");
	redTeam4.SetScale(Scale, Scale);


	blueTeam0 = GUISprite(PositionBlue);
	blueTeam0.Load(GPU::device, L"../Sprites/team_b_0.png");
	blueTeam0.SetScale(Scale, Scale);

	blueTeam1 = GUISprite(PositionBlue);
	blueTeam1.Load(GPU::device, L"../Sprites/team_b_1.png");
	blueTeam1.SetScale(Scale, Scale);

	blueTeam2 = GUISprite(PositionBlue);
	blueTeam2.Load(GPU::device, L"../Sprites/team_b_2.png");
	blueTeam2.SetScale(Scale, Scale);

	blueTeam3 = GUISprite(PositionBlue);
	blueTeam3.Load(GPU::device, L"../Sprites/team_b_3.png");
	blueTeam3.SetScale(Scale, Scale);

	blueTeam4 = GUISprite(100 + 100, 100);
	blueTeam4.Load(GPU::device, L"../Sprites/team_b_4.png");
	blueTeam4.SetScale(Scale, Scale);

}

HudUI::~HudUI()
{

}

void HudUI::Draw()
{
	GUI::Begin();
	SpritePass();
	GUI::End();
}