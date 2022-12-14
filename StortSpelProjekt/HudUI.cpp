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

	if (Input::KeyDown(KeyCode::TAB) || state.IsStartPressed())
	{
		blackBackground.Draw();
		if (state.IsConnected())
		{
			control2.Draw();
		}
		else
		{
			control.Draw();
			useText.Draw();
			throwText.Draw();
			pickText.Draw();
		}
		objective.Draw();
	}

	state.IsConnected() ? controls2.Draw() : controls.Draw();
}



HudUI::HudUI()
{
	using namespace DirectX::SimpleMath;

	#define Scale 0.4f
	#define PositionRed Vector2(50, 70)
	#define PositionBlue Vector2(125, 70)
	#define Max Vector2(125, 90)
	#define Min Vector2(125, 550)
	#define upp 50
	#define left 350
	#define scaleFactor 0.5f
	#define RDOT1 Vector2(310 + left +130, 300 + upp +15)
    #define RDOT2 Vector2(310 + left +170, 300 + upp +15)
	#define SDOT1 Vector2(310 + left + 90, 300 + upp +15)
	#define SDOT2 Vector2(310 + left + 130 , 300 + upp + 15)
#define centerX 632
#define centerY 340
#define offsetY 250 
#define offsetX 50
#define scale 0.3f

	//KTH BARs
	bar_b = GUISprite(centerX, 320 - offsetY);
	bar_b.Load(L"../Sprites/bar/background.png");
	bar_b.SetScale(scale, scale);

	bar_e = GUISprite(centerX, 320 - offsetY);
	bar_e.Load(L"../Sprites/bar/edge.png");
	bar_e.SetScale(scale, scale);

	bar_p = GUISprite(centerX - 105, 320 - offsetY - 12);
	bar_p.Load(L"../Sprites/bar/progress.png");
	bar_p.SetScale(50, scale);
	bar_p.SetOrigin({ 0,bar_p.GetHeight() * 0.5f });
	bar_p.SetTint(DirectX::Colors::Red.v);


	bar_b2 = GUISprite(centerX, 280 - offsetY);
	bar_b2.Load(L"../Sprites/bar/background.png");
	bar_b2.SetScale(scale, scale);

	bar_e2 = GUISprite(centerX, 280 - offsetY);
	bar_e2.Load(L"../Sprites/bar/edge.png");
	bar_e2.SetScale(scale, scale);

	bar_p2 = GUISprite(centerX - 105, 280 - offsetY - 12);
	bar_p2.Load(L"../Sprites/bar/progress.png");
	bar_p2.SetScale(50, scale);
	bar_p2.SetOrigin({ 0, bar_p2.GetHeight() * 0.5f });
	bar_p2.SetTint(DirectX::Colors::Blue.v);


	//ready set go
	ready = GUISprite(310 + left, 300 + upp);
	ready.Load(L"../Sprites/ReadyText.png");
	ready.SetScale(1.2f, 1.2f);

	set = GUISprite(310 + left, 300 + upp);
	set.Load(L"../Sprites/Set.png");
	set.SetScale(1.2f, 1.2f);

	go = GUISprite(310 + left, 300 + upp);
	go.Load(L"../Sprites/GoText.png");
	go.SetScale(1.2f, 1.2f);

	//..!
	dot1 = GUISprite(RDOT1);
	dot1.Load(L"../Sprites/punkt.png");
	dot1.SetScale(0.6f, 0.6f);

	dot2 = GUISprite(RDOT2);
	dot2.Load(L"../Sprites/punkt.png");
	dot2.SetScale(0.6f, 0.6f);

	exclamation = GUISprite(310 + left + 70, 300 + upp);
	exclamation.Load(L"../Sprites/utrop.png");
	exclamation.SetScale(1.2f, 1.2f);

	fade = GUISprite(Vector2(632, 340)); //fadeout
	fade.Load(GPU::device, L"../Sprites/skybox.png");
	fade.SetScale(1.f, 1.f);
	this->setOpacity(true);

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


	control = GUISprite(310 + left, 225 + upp);
	control.Load(L"../Sprites/control.png");
	control.SetScale(0.75f * scaleFactor, 0.75f * scaleFactor);

	control2 = GUISprite(310 + left, 250 + upp);
	control2.Load(L"../Sprites/control2.png");
	control2.SetScale(1.1f * scaleFactor, 1.1f * scaleFactor);

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

	kthObjective = GUISprite(632, 100 + upp);
	kthObjective.Load(L"../Sprites/ObjectiveKTH.png");
	kthObjective.SetScale(0.75f , 0.75f );

	blackBackground = GUISprite(310 + left, 300 + upp);
	blackBackground.Load(L"../Sprites/BackHudTransparent.png");
	blackBackground.SetScale(1.0f, 1.0f);

	controls = GUISprite(1150,650);
	controls.Load(L"../Sprites/DisplayControls.png");
	controls.SetScale(1.0f, 1.0f);

	controls2 = GUISprite(1150 - 50, 650);
	controls2.Load(L"../Sprites/DisplayControls2.png");
	controls2.SetScale(1.0f, 1.0f);


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
	state = gamepad->GetState(0);
	static float speed = 1.0f;
	static float counterDown = 0;
	if (counterDown<=0)
	{
		counterDown = (float)(rand() % 3 + 1);
		speed = 1.0f * counterDown;
	}
	counterDown -= Time::DeltaTimeInSeconds();

	//std::cout << counterDown<< std::endl;

	// fade
	if (Input::KeyDown(KeyCode::G))
	{
		this->fadeIn();
	}
	// fade
	if (Input::KeyDown(KeyCode::H))
	{
		this->fadeOut();
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
		done = true;
	}
	else
	{
		fade.SetTint(gg);
	}
	return done;
}

void HudUI::displayObjective()
{
	if (!done)
	{
		GUI::Begin();
		blackBackground2.Draw();
		objective2.Draw();

		GUI::End();
	}
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

bool HudUI::readySetGo()
{
	using namespace DirectX::SimpleMath;
	handle = true;

	done = false;
	readyTimer += Time::DeltaTimeInSeconds();

	blackBackground.SetScale(4.0f, 4.0f);

	if (readyTimer >= 1)
	{
		this->Bready = true;
	}
	if (readyTimer >= 1.7f && readyTimer <= 2.5f)
	{
		//draw dot 1
		Bdot1 = true;
	}
	if (readyTimer >= 2.1f && readyTimer <= 2.5f)
	{
		//draw dot 2
		Bdot2 = true;
	}
	if (readyTimer >= 2.5f)
	{
		Bdot1 = false;
		Bdot2 = false;
		this->Bready = false;
		this->Bset = true;
		this->dot1.SetPosition(Vector2(310 + left + 90, 300 + upp + 15));
		this->dot2.SetPosition(Vector2(310 + left + 130, 300 + upp + 15));
	}
	if (readyTimer >= 2.7f && readyTimer <= 3.5f)
	{
		//draw dot 1
		Bdot1 = true;
	}
	if (readyTimer >= 3.1f && readyTimer <= 3.5f)
	{
		//draw dot 2
		Bdot2 = true;
	}
	if (readyTimer >= 3.5f)
	{
		Bdot1 = false;
		Bdot2 = false;
		this->Bset = false;
		this->Bgo = true;
	}
	if (readyTimer >= 4.5f)
	{
		this->Bgo = false;
		done = true;
		handle = false;
	}
	return done;
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

void HudUI::SetGamePad(DirectX::GamePad* g)
{
	gamepad = g;
}

void HudUI::DrawFade()
{
#define barvalue 53.0f
	bar_p.SetScale((sin(Time::CurrentTime()) * 0.5f + 0.5f) * barvalue, 0.3f);
	bar_p2.SetScale((sin(Time::CurrentTime()) * 0.5f + 0.5f) * barvalue, 0.3f);

	handleInputs();
	GUI::Begin();
	fade.Draw();

	if (kth)
	{
		bar_b.Draw();
		bar_p.Draw();
		bar_e.Draw();
		bar_b2.Draw();
		bar_p2.Draw();
		bar_e2.Draw();
	}



	if (handle)
	{
		blackBackground.Draw();
	}
	if (Bready)
	{
		ready.Draw();
	}
	else if (Bset)
	{
		set.Draw();
	}
	else if (Bgo)
	{
		go.Draw();
		exclamation.Draw();
	}
	if (Bdot1)
	{
		dot1.Draw();
	}
	if (Bdot2)
	{
		dot2.Draw();
	}

	if (!done && kth)
	{
		blackBackground2.Draw();
		kthObjective.Draw();
	}

	if (Input::KeyDown(KeyCode::TAB) || state.IsStartPressed())
	{
		blackBackground.Draw();
		if (state.IsConnected())
		{
			control2.Draw();
		}
		else
		{
			control.Draw();
			useText.Draw();
			throwText.Draw();
			pickText.Draw();
		}
		objective.Draw();
	}
	state.IsConnected() ? controls2.Draw() : controls.Draw();

	GUI::End();
}

void HudUI::resetReadySetGo()
{
	readyTimer = 0;
	done = Bdot2 = Bdot1 = Bgo = Bset = Bready = handle = false;
}

void HudUI::Draw()
{
	handleInputs();
	GUI::Begin();
	SpritePass();
	GUI::End();
	DrawFade();
	displayObjective();
}

void HudUI::setKTH(bool stuff)
{
	kth = stuff;
}

bool HudUI::isDone() const
{
	return this->done;
}