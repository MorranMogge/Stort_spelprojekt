#pragma once
#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"
#include <GamePad.h>
#include "TimeStruct.h"

class MiniGameUI
{
	GUISprite bar_b, bar_e, bar_p;
	GUISprite bar_b2, bar_e2, bar_p2;
	GUISprite L_wheel, arrowUp, arrowDown;

	GUISprite scoreText, scoreText2;
	GUISprite n0, n1, n2, n3, n4, n5, n6, n7, n8, n9;

public:
	float pointRed = 0, pointBlue = 0;
	MiniGameUI();
	~MiniGameUI();

	void setPointRed(const float point);
	void setPointBlue(const float point);

	void HandleInputs();
	void SpritePass();
	void Draw();
};