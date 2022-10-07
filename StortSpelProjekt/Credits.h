#pragma once
#include "GUISprite.h"
#include "State.h"

class Credits final
{

	GUISprite creditsText;
	GUISprite nameText;
	GUISprite backText;

	GAMESTATE gameState;

	void HandleInputs();

	void TextPass();
	void SpritePass();

public:

	GAMESTATE GetGameState();

	Credits();
	~Credits();

	void Draw();
};

