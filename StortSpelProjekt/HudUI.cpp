#include "stdafx.h"
#include "HudUI.h"
#include "Input.h"
#include "SpaceShip.h"


void HudUI::SpritePass()
{


	if (red)
	{
		switch (red->getNrOfComponents())
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

	if (blue)
	{
		switch (blue->getNrOfComponents())
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

	if (Input::KeyDown(KeyCode::TAB))
	{
		blackBackground.Draw();
		control.Draw();
		objective.Draw();
		useText.Draw();
		throwText.Draw();
		pickText.Draw();
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

	blueTeam4 = GUISprite(PositionBlue);
	blueTeam4.Load(GPU::device, L"../Sprites/team_b_4.png");
	blueTeam4.SetScale(Scale, Scale);

	#define upp 50
	#define left 350

	#define scaleFactor 0.5f
	control = GUISprite(310 + left, 225 + upp);
	control.Load(GPU::device, L"../Sprites/control.png");
	control.SetScale(0.75f * scaleFactor, 0.75f * scaleFactor);

	useText = GUISprite(317 + left, 320 + upp);
	useText.Load(GPU::device, L"../Sprites/UseText.png");
	useText.SetScale(0.40f * scaleFactor, 0.40f * scaleFactor);

	throwText = GUISprite(327 + left, 360 + upp);
	throwText.Load(GPU::device, L"../Sprites/ThrowText.png");
	throwText.SetScale(0.40f * scaleFactor, 0.40f * scaleFactor);

	pickText = GUISprite(318 + left, 400 + upp);
	pickText.Load(GPU::device, L"../Sprites/PickText.png");
	pickText.SetScale(0.40f * scaleFactor, 0.40f * scaleFactor);

	objective = GUISprite(310 + left, 450 + upp);
	objective.Load(GPU::device, L"../Sprites/Objective.png");
	objective.SetScale(0.75f * scaleFactor, 0.75f * scaleFactor);

	blackBackground = GUISprite(310 + left, 300 + upp);
	blackBackground.Load(GPU::device, L"../Sprites/BackHudTransparent.png");
	blackBackground.SetScale(1.0f, 1.0f);


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