#pragma once
#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"

class MenuUI final
{
	GUISprite spriteWeaponBox;

	GUISprite start;
	GUISprite settings;
	GUISprite credits;
	GUISprite exit;

	GUISprite control;
	GUISprite useText;
	GUISprite throwText;
	GUISprite pickText;

	GUISprite objective;

	bool isLoading = false;
	GUISprite Loading;

	GAMESTATE gameState;

	GUIText testText;

	void HandleInputs();

	void TextPass();
	void SpritePass();

public:

	GAMESTATE GetGameState();

	MenuUI();
	void Draw();

};
