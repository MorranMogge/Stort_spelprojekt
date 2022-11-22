#pragma once
#include <SimpleMath.h>

#include "TimeStruct.h"
#include "GUISprite.h"
#include "GUIText.h"

#define MaxVec DirectX::SimpleMath::Vector2(125, 90)
#define MinVec DirectX::SimpleMath::Vector2(125, 550)

class LandingHud
{
private:
	GUISprite landing0;
	GUISprite landing1;
	GUISprite landing2;

	TimeStruct timer;


public:
	LandingHud();
	~LandingHud();

	bool handleInputs(const float& dt);
	void draw();
};