#pragma once
#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"
#include <GamePad.h>

struct MiniGameUI
{
	GUISprite bar_b, bar_e, bar_p;

	GUISprite n0, n1, n2, n3, n4, n5, n6, n7, n8, n9;

	MiniGameUI();
	~MiniGameUI();

	void HandleInputs();
	void SpritePass();
	void Draw();
};