#pragma once

#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"

class LoseUI final
{

	GUISprite loseText;
	GUISprite backText;

	GAMESTATE gameState;

	void HandleInputs();
	void SpritePass();


public:
	GAMESTATE GetGameState();

	LoseUI();
	~LoseUI();

	void Draw();
};
