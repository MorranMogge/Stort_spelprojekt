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

	GUISprite LB, LT, L_Move;

	GUISprite RB, RT, R_Move;

	GUISprite start;

	GUISprite backText;

	GAMESTATE gameState = NOCHANGE;

	std::unique_ptr<DirectX::GamePad> gamePad;

	void HandleInputs();
	void SpritePass();


public:
	GAMESTATE GetGameState();

	ControlUI();
	~ControlUI();

	void Draw();
};
