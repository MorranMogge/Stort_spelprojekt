#pragma once
#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"

class GuiHandler final
{
	GUISprite spriteWeaponBox;

	GUISprite start;
	GUISprite settings;
	GUISprite credits;
	GUISprite exit;

	GAMESTATE gameState;

	GUIText testText;

	void HandleInputs();

	void TextPass();
	void SpritePass();

public:

	GAMESTATE GetGameState();

	GuiHandler();
	void Draw();

};

