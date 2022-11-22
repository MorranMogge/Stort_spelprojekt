#pragma once

#include "GUISprite.h"
#include "State.h"
#include <GamePad.h>

class ControlUI final
{
	GUISprite control;
	GUISprite useText;
	GUISprite throwText;
	GUISprite pickText;

	GUISprite a, b, y, x;
	GUISprite arrow;

	GUISprite LB, LT, L_wheel;

	GUISprite RB, RT, R_wheel;

	GUISprite start;

	GUISprite backText;

	GUISprite usePickText,throwText2,moveText2;

	GAMESTATE gameState = NOCHANGE;

	GUISprite objective;

	std::unique_ptr<DirectX::GamePad> gamePad;

	void HandleInputs();
	void SpritePass();


public:
	GAMESTATE GetGameState();

	ControlUI();
	~ControlUI();

	void Draw();
};
