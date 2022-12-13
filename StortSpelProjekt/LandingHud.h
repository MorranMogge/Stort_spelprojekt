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

	GUISprite wheel_L, arrow, arrow2;
	GUISprite Skey, Wkey;
	GUISprite scoreText, scoreText2;
	GUISprite n0, n1, n2, n3, n4, n5, n6, n7, n8, n9;

	DirectX::GamePad* gamePad;
	DirectX::GamePad::ButtonStateTracker tracker;
	DirectX::GamePad::State state;

	TimeStruct timer;

	float targetTime;
	float currentTime;
	float factor;
	float setScore = false;

	bool show_gamepad = false;

	int ggop = 0;

	DirectX::SimpleMath::Vector2 changePos;
	DirectX::SimpleMath::Vector2 changeScale;

public:
	LandingHud();
	~LandingHud();
	void setLandingScore(int score);
	void makeGamePad(DirectX::GamePad* gamePad);
	bool handleInputs(const float& dt);
	void draw();
	void drawPointsForOtherGameModes(const int scores[]);
};