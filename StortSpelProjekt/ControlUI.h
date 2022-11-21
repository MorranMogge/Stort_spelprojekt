#pragma once

#include "GUISprite.h"
#include "State.h"

class ControlUI final
{
	GUISprite control;
	GUISprite useText;
	GUISprite throwText;
	GUISprite pickText;

	GUISprite backText;

	GAMESTATE gameState;

	void HandleInputs();
	void SpritePass();


public:
	GAMESTATE GetGameState();

	ControlUI();
	~ControlUI();

	void Draw();
};
