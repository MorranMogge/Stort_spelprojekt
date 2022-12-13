#pragma once

#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"

class InputIPUI final
{

	GUISprite backText;

	GUISprite n0, n1, n2, n3, n4, n5, n6, n7, n8, n9;
	GUISprite punkt;

	GUISprite start;
	GUISprite start2;

	GUISprite ipText;

	bool hit_start = false;

	GAMESTATE gameState = NOCHANGE;

	void HandleInputs();
	void SpritePass();


public:
	inline static std::string ip = "192.168.1.123";
	GAMESTATE GetGameState();

	InputIPUI();
	~InputIPUI();

	void Draw();
};