#include "stdafx.h"
#include "LandingHud.h"
#include "Input.h"

LandingHud::LandingHud()
	:factor(1.0f)
{
	landing0 = GUISprite(DirectX::SimpleMath::Vector2(125, 320)); //bar
	landing0.Load(L"../Sprites/Bar.png");
	landing0.SetScale(0.51f, 0.51f);

	landing1 = GUISprite(125, (MinVec.y - MaxVec.y) / 2); //safebox
	landing1.Load(L"../Sprites/safeBox.png");
	landing1.SetScale(0.2f, 0.06f);

	landing2 = GUISprite(125, (MinVec.y - MaxVec.y) / 2); //ship
	landing2.Load(L"../Sprites/ship.png");
	landing2.SetScale(0.2f, 0.2f);

	wheel_L = GUISprite(325, 320);
	wheel_L.Load(L"../Sprites/control/L_wheel.png");
	wheel_L.SetScale(0.4f, 0.4f);

	arrow = GUISprite(325, 430);
	arrow.Load(L"../Sprites/arrow_g.png");
	arrow.SetScale(0.4f, 0.4f);
	arrow.SetRotation(3.1415926f);

	arrow2 = GUISprite(325, 210);
	arrow2.Load(L"../Sprites/arrow_o.png");
	arrow2.SetScale(0.4f, 0.4f);

	Skey = GUISprite(325, 350);
	Skey.Load(L"../Sprites/Skey.png");
	Skey.SetScale(0.4f, 0.4f);

	Wkey = GUISprite(325, 290);
	Wkey.Load(L"../Sprites/Wkey.png");
	Wkey.SetScale(0.4f, 0.4f);

	this->targetTime = 0;
	this->currentTime = 0;
}

LandingHud::~LandingHud()
{
}

void LandingHud::makeGamePad(DirectX::GamePad* gamePad)
{
	this->gamePad = gamePad;
}

bool LandingHud::handleInputs(const float& dt)
{
	bool intersected = false;

	this->state = gamePad->GetState(0);
	tracker.Update(state);
	
	if (this->currentTime >= this->targetTime)
	{
		this->targetTime = factor * (rand() % 21)*0.1f + 0.75f;
		this->currentTime = 0;

		float realTemp = (float)(rand() % 360) * (DirectX::XM_PI / 180.f);
		float sinCurve = (sin(realTemp) * 0.5f) + 0.5f;
		DirectX::SimpleMath::Vector2 targetPos;
		targetPos.y = MaxVec.y + sinCurve * (MinVec.y - MaxVec.y);

		realTemp = (float)(rand() % 360) * (DirectX::XM_PI / 180.f);
		DirectX::SimpleMath::Vector2 targetScale;
		targetScale.y = abs(sin(realTemp)) * 0.15f + 0.1f;

		this->changePos = (targetPos - this->landing1.GetPosition())/this->targetTime;
		this->changeScale = (targetScale - this->landing1.GetScale()) / this->targetTime;

	}

	this->currentTime += dt;

	float speed = 300.f;

	DirectX::SimpleMath::Vector2 rocketPos = landing2.GetPosition();
	DirectX::SimpleMath::Vector2 safePos = landing1.GetPosition();
	
	//Move "safezone" sprite
	landing1.SetPosition(DirectX::SimpleMath::Vector2(MaxVec.x, landing1.GetPosition().y + changePos.y * dt));
	landing1.SetScale(DirectX::SimpleMath::Vector2(0.2f, landing1.GetScale().y + changeScale.y * dt));

	if (state.IsConnected())
	{
		show_gamepad = true;
		if (abs(state.thumbSticks.leftY) > 0)
		{
			DirectX::SimpleMath::Vector2 test(rocketPos.x, rocketPos.y - dt * speed * state.thumbSticks.leftY);

			if (rocketPos.y < MaxVec.y - 25)
			{
				landing2.SetPosition(DirectX::SimpleMath::Vector2(MaxVec.x, rocketPos.y));
			}
			else
			{
				landing2.SetPosition(test);
			}
		}
	}
	else
	{
		show_gamepad = false;
		//Move player sprite
		if (Input::KeyDown(KeyCode::W) || Input::KeyDown(KeyCode::ARROW_Up))
		{
			DirectX::SimpleMath::Vector2 test(rocketPos.x, rocketPos.y - dt * speed);

			if (rocketPos.y < MaxVec.y - 25)
			{
				landing2.SetPosition(DirectX::SimpleMath::Vector2(MaxVec.x, rocketPos.y));
			}
			else
			{
				landing2.SetPosition(test);
			}
		}
		if (Input::KeyDown(KeyCode::S) || Input::KeyDown(KeyCode::ARROW_Down))
		{
			DirectX::SimpleMath::Vector2 test(rocketPos.x, rocketPos.y + dt * speed);

			if (rocketPos.y > MinVec.y + 25)
			{
				landing2.SetPosition(DirectX::SimpleMath::Vector2(MaxVec.x, rocketPos.y));
			}
			else
			{
				landing2.SetPosition(test);
			}
		}
	}


	//Check sprite intersection
	if (landing1.IntersectSprite(landing2))
	{
		intersected = true;
	}

	return intersected;
}

void LandingHud::draw()
{
	GUI::Begin();
	
	landing0.Draw();
	landing1.Draw();
	landing2.Draw();

	if (show_gamepad)
	{
		wheel_L.Draw();
		arrow.Draw();
		arrow2.Draw();
	}
	else
	{
		Skey.Draw();
		Wkey.Draw();
		arrow.Draw();
		arrow2.Draw();
	}


	GUI::End();
}
