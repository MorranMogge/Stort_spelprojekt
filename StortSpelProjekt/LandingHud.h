#pragma once
#include <SimpleMath.h>

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

	TimeStruct timer;

	float targetTime;
	float currentTime;
	float factor;

	DirectX::SimpleMath::Vector2 changePos;
	DirectX::SimpleMath::Vector2 changeScale;

public:
	LandingHud();
	~LandingHud();

	bool handleInputs(const float& dt);
	void draw();
};