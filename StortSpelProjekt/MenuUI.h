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

	GUISprite start2;
	GUISprite settings2;
	GUISprite credits2;
	GUISprite exit2;

	bool hit_start, hit_setting, hit_credits, hit_exit;

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

