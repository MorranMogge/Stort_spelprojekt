#include "stdafx.h"
#include "HudUI.h"
#include "Input.h"
#include "SpaceShip.h"
#include "Time.h"

void setScaleOverTime(bool& changed, GUISprite &sprite, float time = 0.3)
{
	//start timer & set scale, revert to normal scale after timer.
	static float scaleTimer;

	//if changed sprite start timer
	if (scaleTimer < time && changed)		//runs every frame untill timer runs out
	{
		scaleTimer += Time::DeltaTimeInSeconds();
		sprite.SetScale(0.5f, 0.5f);
	}
	else if (scaleTimer > time && changed)	//run once when timer runs out
	{
		scaleTimer = 0;
		changed = false;
		sprite.SetScale(0.4f, 0.4f);
	}
}

void HudUI::SpritePass()
{
	if (Input::KeyDown(KeyCode::B))//landing
	{
		landing0.Draw();
		landing1.Draw();
		landing2.Draw();
	}

	static bool red1 = true;
	static bool red2 = true;
	static bool red3 = true;
	static bool red4 = true;

	if (red)
	{
		switch (red->getNrOfComponents())
		{
		case 0:
			redTeam0.Draw(); break;
		case 1:
			setScaleOverTime(red1, redTeam1);
			redTeam1.Draw(); break;
		case 2:
			setScaleOverTime(red2, redTeam2);
			redTeam2.Draw(); break;
		case 3:
			setScaleOverTime(red3, redTeam3);
			redTeam3.Draw(); break;
		case 4:
			setScaleOverTime(red4, redTeam4);
			redTeam4.Draw(); break;
		}
	}

	static bool blue1 = true;
	static bool blue2 = true;
	static bool blue3 = true;
	static bool blue4 = true;

	if (blue)
	{
		switch (blue->getNrOfComponents())
		{
		case 0:
			blueTeam0.Draw(); break;
		case 1:
			setScaleOverTime(blue1, blueTeam1);
			blueTeam1.Draw(); break;
		case 2:
			setScaleOverTime(blue2, blueTeam2);
			blueTeam2.Draw(); break;
		case 3:
			setScaleOverTime(blue3, blueTeam3);
			blueTeam3.Draw(); break;
		case 4:
			setScaleOverTime(blue4, blueTeam4);
			blueTeam4.Draw(); break;
		}
	}

	if (player)
	{
		switch (player->isHoldingItem())
		{
		case 1:
			useControls.Draw();
			useControls1.Draw(); 
			useControls2.Draw(); break;
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

	if (Input::KeyDown(KeyCode::G))
	{
		this->fadeIn();
	}
	if (Input::KeyDown(KeyCode::H))
	{
		this->fadeOut();
	}

	if (objectiveDisplayTime > 0)
	{
		objectiveDisplayTime -= Time::DeltaTimeInSeconds();
		blackBackground2.Draw();
		objective2.Draw();
	}


	
	controls.Draw();
	fade.Draw();
}

HudUI::HudUI()
{
	using namespace DirectX::SimpleMath;

	#define Scale 0.4f
	#define PositionRed Vector2(50, 70)
	#define PositionBlue Vector2(125, 70)
	#define Max Vector2(125, 90)
	#define Min Vector2(125, 550)
	timer.startTime;

	fade = GUISprite(Vector2(125, 320)); //fadeout
	fade.Load(GPU::device, L"../Sprites/skybox.jpg");
	fade.SetScale(1.f, 1.f);
	this->setOpacity(true);
	
	landing0 = GUISprite(Vector2(125, 320)); //bar
	landing0.Load(L"../Sprites/Bar.png");
	landing0.SetScale(0.51f, 0.51f);

	landing1 = GUISprite(125, (Min.y - Max.y) / 2); //safebox
	landing1.Load(L"../Sprites/safeBox.png");
	landing1.SetScale(0.2f, 0.06f);

	landing2 = GUISprite(125,(Min.y - Max.y)/2); //ship
	landing2.Load(L"../Sprites/ship.png");
	landing2.SetScale(0.2f, 0.2f);

	redTeam0 = GUISprite(PositionRed);
	redTeam0.Load(L"../Sprites/r_0.png");
	redTeam0.SetScale(Scale, Scale);

	redTeam1 = GUISprite(PositionRed);
	redTeam1.Load(L"../Sprites/r_1.png");
	redTeam1.SetScale(Scale, Scale);

	redTeam2 = GUISprite(PositionRed);
	redTeam2.Load(L"../Sprites/r_2.png");
	redTeam2.SetScale(Scale, Scale);

	redTeam3 = GUISprite(PositionRed);
	redTeam3.Load(L"../Sprites/r_3.png");
	redTeam3.SetScale(Scale, Scale);

	redTeam4 = GUISprite(PositionRed);
	redTeam4.Load(L"../Sprites/r_4.png");
	redTeam4.SetScale(Scale, Scale);


	blueTeam0 = GUISprite(PositionBlue);
	blueTeam0.Load(L"../Sprites/b_0.png");
	blueTeam0.SetScale(Scale, Scale);

	blueTeam1 = GUISprite(PositionBlue);
	blueTeam1.Load(L"../Sprites/b_1.png");
	blueTeam1.SetScale(Scale, Scale);

	blueTeam2 = GUISprite(PositionBlue);
	blueTeam2.Load(L"../Sprites/b_2.png");
	blueTeam2.SetScale(Scale, Scale);

	blueTeam3 = GUISprite(PositionBlue);
	blueTeam3.Load(L"../Sprites/b_3.png");
	blueTeam3.SetScale(Scale, Scale);

	blueTeam4 = GUISprite(PositionBlue);
	blueTeam4.Load(L"../Sprites/b_4.png");
	blueTeam4.SetScale(Scale, Scale);

	#define upp 50
	#define left 350

	#define scaleFactor 0.5f
	control = GUISprite(310 + left, 225 + upp);
	control.Load(L"../Sprites/control.png");
	control.SetScale(0.75f * scaleFactor, 0.75f * scaleFactor);

	useText = GUISprite(317 + left, 320 + upp);
	useText.Load(L"../Sprites/UseText.png");
	useText.SetScale(0.40f * scaleFactor, 0.40f * scaleFactor);

	throwText = GUISprite(327 + left, 360 + upp);
	throwText.Load(L"../Sprites/ThrowText.png");
	throwText.SetScale(0.40f * scaleFactor, 0.40f * scaleFactor);

	pickText = GUISprite(318 + left, 400 + upp);
	pickText.Load(L"../Sprites/PickText.png");
	pickText.SetScale(0.40f * scaleFactor, 0.40f * scaleFactor);

	objective = GUISprite(310 + left, 450 + upp);
	objective.Load(L"../Sprites/Objective.png");
	objective.SetScale(0.75f * scaleFactor, 0.75f * scaleFactor);

	blackBackground = GUISprite(310 + left, 300 + upp);
	blackBackground.Load(L"../Sprites/BackHudTransparent.png");
	blackBackground.SetScale(1.0f, 1.0f);

	controls = GUISprite(1150,650);
	controls.Load(L"../Sprites/DisplayControls.png");
	controls.SetScale(1.0f, 1.0f);


	useControls = GUISprite(390 + left, 600 + upp);
	useControls.Load(L"../Sprites/ThrowText2.png");
	useControls.SetScale(0.40f * scaleFactor, 0.40f * scaleFactor);

	useControls1 = GUISprite(280 + left, 600 + upp);
	useControls1.Load(L"../Sprites/control/use.png");
	useControls1.SetScale(0.50f * scaleFactor, 0.50f * scaleFactor);

	useControls2 = GUISprite(240 + left, 600 + upp);
	useControls2.Load(L"../Sprites/Ekey.png");
	useControls2.SetScale(0.45f * scaleFactor, 0.45f * scaleFactor);

	objective2 = GUISprite(632, 100 + upp);
	objective2.Load(L"../Sprites/Objective.png");
	objective2.SetScale(0.75f, 0.75f);

	blackBackground2 = GUISprite(632, 100 + upp);
	blackBackground2.Load(L"../Sprites/BackHudTransparent.png");
	blackBackground2.SetScale(1.0f, 0.3f);
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
	float sinCurve = (sin(timer.getDt())*0.5f) + 0.5f;


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

bool HudUI::fadeIn()
{	
	bool done = false;
	DirectX::SimpleMath::Color gg = fade.GetTint();
	count -= Time::DeltaTimeInSeconds() + 0.007f;
	gg.w = count;
	if (gg.w <=0)
	{
		std::cout << "what?" << std::endl;
		done = true;
	}
	else
	{
		fade.SetTint(gg);
	}
	return done;
}

void HudUI::fadeOut()
{
	DirectX::SimpleMath::Color gg = fade.GetTint();
	gg.w += Time::DeltaTimeInSeconds() + 0.005f;
	fade.SetTint(gg);
}

void HudUI::moveSprite()
{
	using namespace DirectX::SimpleMath;
	
	if (blue->getCompletion())
	{
		Vector2 currentPos = blueTeam4.GetPosition();
		blueTeam4.SetPosition(Vector2(currentPos.x, currentPos.y - 0.5f));
	}
	else if (red->getCompletion())
	{
		Vector2 currentPos = redTeam4.GetPosition();
		redTeam4.SetPosition(Vector2(currentPos.x, currentPos.y - 0.5f));
	}
}

void HudUI::setOpacity(bool onOff)
{
	DirectX::SimpleMath::Color opacity = fade.GetTint();
	if (onOff)
	{
		opacity.w = 0.0f;
	}
	else
	{
		opacity.w = 1.0f;
	}
	fade.SetTint(opacity);
}

void HudUI::Draw()
{
	GUI::Begin();
	SpritePass();
	if (Input::KeyDown(KeyCode::B)){ handleInputs(); }
	//
	GUI::End();
}