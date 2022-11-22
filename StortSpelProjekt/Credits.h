#pragma once
#include "GUISprite.h"
#include "State.h"
#include <GamePad.h>

class Credits final
{

	GUISprite creditsText;
	GUISprite nameText;
	GUISprite backText;

	GAMESTATE gameState;

	std::unique_ptr<DirectX::GamePad> gamePad;

	void HandleInputs();

	void TextPass();
	void SpritePass();

public:

	GAMESTATE GetGameState();

	Credits();
	~Credits();

	void Draw();
};

