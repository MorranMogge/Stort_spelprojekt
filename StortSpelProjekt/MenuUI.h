#pragma once
#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"
#include <GamePad.h>

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

	GUISprite bar_b, bar_e, bar_p;

	bool isLoading = false;
	GUISprite Loading;

	GAMESTATE gameState = NOCHANGE;

	int selectIndex = 0;

	std::unique_ptr<DirectX::GamePad> gamePad;

	void HandleInputs();
	void SpritePass();

public:

	GAMESTATE GetGameState();

	MenuUI();
	void Draw();

};

