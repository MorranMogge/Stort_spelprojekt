#pragma once

#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"

class WinUI final
{

	GUISprite winText;
	GUISprite backText;

	GAMESTATE gameState;

	void HandleInputs();
	void SpritePass();


public:
	GAMESTATE GetGameState();

	WinUI();
	~WinUI();

	void Draw();
};
