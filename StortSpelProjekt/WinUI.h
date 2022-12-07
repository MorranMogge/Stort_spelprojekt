#pragma once

#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"
#include <GamePad.h>

class WinUI final
{

	GUISprite winText;
	GUISprite backText;

	GAMESTATE gameState;

	std::unique_ptr<DirectX::GamePad> gamePad;

	void HandleInputs();
	void SpritePass();


public:
	GAMESTATE GetGameState();

	WinUI();
	~WinUI();

	void Draw();
};
