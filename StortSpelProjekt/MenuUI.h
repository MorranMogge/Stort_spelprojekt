#pragma once
#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"

class MenuUI final
{
	GUISprite spriteWeaponBox;

	GUISprite start;
	GUISprite control;
	GUISprite settings;
	GUISprite credits;
	GUISprite exit;

	GUISprite start2;
	GUISprite control2;
	GUISprite settings2;
	GUISprite credits2;
	GUISprite exit2;

	bool hit_start, hit_setting, hit_credits, hit_exit, hit_control;

	GUISprite title;

	GUISprite objective;

	bool isLoading = false;
	GUISprite Loading;

	GAMESTATE gameState = NOCHANGE;

	GUIText testText;

	void HandleInputs();
	void SpritePass();

public:

	GAMESTATE GetGameState();

	MenuUI();
	void Draw();

};

