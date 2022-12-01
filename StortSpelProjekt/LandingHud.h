#pragma once
#include <SimpleMath.h>
#include <GamePad.h>

#include "TimeStruct.h"
#include "GUISprite.h"
#include "GUIText.h"

#define MaxVec DirectX::SimpleMath::Vector2(125, 100)
#define MinVec DirectX::SimpleMath::Vector2(125, 540)

class LandingHud
{
private:
	GUISprite landing0;
	GUISprite landing1;
	GUISprite landing2;

	DirectX::GamePad* gamePad;
	DirectX::GamePad::ButtonStateTracker tracker;
	DirectX::GamePad::State state;

	TimeStruct timer;

	float targetTime;
	float currentTime;
	float factor;

	DirectX::SimpleMath::Vector2 changePos;
	DirectX::SimpleMath::Vector2 changeScale;

public:
	LandingHud();
	~LandingHud();

	void makeGamePad(DirectX::GamePad* gamePad);
	bool handleInputs(const float& dt);
	void draw();
};