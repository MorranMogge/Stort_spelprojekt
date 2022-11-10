#include "stdafx.h"
#include "HudUI.h"
#include "Input.h"
#include "SpaceShip.h"
#include "Time.h"


void HudUI::SpritePass()
{
	if (Input::KeyDown(KeyCode::B))//landing
	{
		landing0.Draw();
		landing1.Draw();
		landing2.Draw();
	}

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

	if (player)
	{
		switch (player->isHoldingItem())
		{
		case 1:
			useControls.Draw();
			useControls1.Draw(); break;
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

	controls.Draw();

}

HudUI::HudUI()
	:landingCounter(0.0f)
{
	using namespace DirectX::SimpleMath;

	#define Scale 0.4f
	#define PositionRed Vector2(50, 620)
	#define PositionBlue Vector2(125, 620)
	#define Max Vector2(125, 90)
	#define Min Vector2(125, 550)
	timer.startTime;

	landing0 = GUISprite(Vector2(125, 320));
	landing0.Load(GPU::device, L"../Sprites/Bar.png");
	landing0.SetScale(0.2f, 0.2f);

	landing1 = GUISprite(125, (Min.y - Max.y) / 2);
	landing1.Load(GPU::device, L"../Sprites/safeBox.png");
	landing1.SetScale(0.2f, 0.06f);

	landing2 = GUISprite(125,(Min.y - Max.y)/2);
	landing2.Load(GPU::device, L"../Sprites/ship.png");
	landing2.SetScale(0.2f, 0.2f);

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

	controls = GUISprite(1150,650);
	controls.Load(GPU::device, L"../Sprites/DisplayControls.png");
	controls.SetScale(1.0f, 1.0f);


	useControls = GUISprite(300 + left, 600 + upp);
	useControls.Load(GPU::device, L"../Sprites/ThrowText.png");
	useControls.SetScale(0.40f * scaleFactor, 0.40f * scaleFactor);

	useControls1 = GUISprite(290 + left, 570 + upp);
	useControls1.Load(GPU::device, L"../Sprites/UseText.png");
	useControls1.SetScale(0.40f * scaleFactor, 0.40f * scaleFactor);
}

HudUI::~HudUI()
{

}

void HudUI::handleInputs()
{
	static float speed = 1.0f;

	static float counterDown = 0;
	if (counterDown<=0)
	{
		counterDown = (float)(rand() % 3 + 1);
		speed = 1.0f * counterDown;
	}
	counterDown -= Time::DeltaTimeInSeconds();

	//std::cout << counterDown<< std::endl;
	using namespace DirectX::SimpleMath;
	Vector2 rocketPos = landing2.GetPosition();
	Vector2 safePos = landing1.GetPosition();
	float sinCurve = (sin(timer.getDt())*0.5) + 0.5;


	//Move "safezone" sprite
	landing1.SetPosition(Vector2(Max.x, 90 + sinCurve * 460));
	
	//Move player sprite
	if (Input::KeyDown(KeyCode::W) || Input::KeyDown(KeyCode::ARROW_Up))
	{
		Vector2 test(rocketPos.x , rocketPos.y - 3);

		if (rocketPos.y < Max.y-25)
		{
			landing2.SetPosition(Vector2(Max.x, rocketPos.y));
		}
		else
		{
			landing2.SetPosition(test);
		}
	}
	if (Input::KeyDown(KeyCode::S) || Input::KeyDown(KeyCode::ARROW_Down))
	{
		Vector2 test(rocketPos.x , rocketPos.y + 3);

		if (rocketPos.y > Min.y + 25)
		{
			landing2.SetPosition(Vector2(Max.x, rocketPos.y));
		}
		else
		{
			landing2.SetPosition(test);
		}
	}
	
	//Check sprite intersection
	if (landing1.IntersectSprite(landing2))
	{
		std::cout << "intersects" << std::endl;
	}
}

void HudUI::Draw()
{
	GUI::Begin();
	SpritePass();
	if (Input::KeyDown(KeyCode::B)){ handleInputs(); }
	//
	GUI::End();
}