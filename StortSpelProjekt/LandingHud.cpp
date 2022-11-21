#include "stdafx.h"
#include "LandingHud.h"
#include "Input.h"

LandingHud::LandingHud()
{
	landing0 = GUISprite(DirectX::SimpleMath::Vector2(125, 320)); //bar
	landing0.Load(GPU::device, L"../Sprites/Bar.png");
	landing0.SetScale(0.51f, 0.51f);

	landing1 = GUISprite(125, (MinVec.y - MaxVec.y) / 2); //safebox
	landing1.Load(GPU::device, L"../Sprites/safeBox.png");
	landing1.SetScale(0.2f, 0.06f);

	landing2 = GUISprite(125, (MinVec.y - MaxVec.y) / 2); //ship
	landing2.Load(GPU::device, L"../Sprites/ship.png");
	landing2.SetScale(0.2f, 0.2f);
}

LandingHud::~LandingHud()
{
}

bool LandingHud::handleInputs(const float& dt)
{
	bool intersected = false;
	float speed = 300.f;

	DirectX::SimpleMath::Vector2 rocketPos = landing2.GetPosition();
	DirectX::SimpleMath::Vector2 safePos = landing1.GetPosition();
	float sinCurve = (sin(timer.getDt()) * 0.5f) + 0.5f;


	//Move "safezone" sprite
	landing1.SetPosition(DirectX::SimpleMath::Vector2(MaxVec.x, 90 + sinCurve * 460));

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

	//Check sprite intersection
	if (landing1.IntersectSprite(landing2))
	{
		intersected = true;
		std::cout << "intersects" << std::endl;
	}

	return intersected;
}

void LandingHud::draw()
{
	GUI::Begin();
	
	landing0.Draw();
	landing1.Draw();
	landing2.Draw();

	GUI::End();
}
