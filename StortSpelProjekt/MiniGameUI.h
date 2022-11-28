#pragma once
#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"
#include <GamePad.h>

struct MiniGameUI
{
	GUISprite bar_b, bar_e, bar_p;

	MiniGameUI();
	~MiniGameUI();

	void HandleInputs();
	void SpritePass();
	void Draw();
};