#pragma once

#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"
#include <GamePad.h>

class LoseUI final
{

	GUISprite loseText;
	GUISprite backText;

	GAMESTATE gameState;

	std::unique_ptr<DirectX::GamePad> gamePad;

	void HandleInputs();
	void SpritePass();


public:
	GAMESTATE GetGameState();

	LoseUI();
	~LoseUI();

	void Draw();
};
