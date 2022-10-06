#pragma once

#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"

class SettingsUI final
{

	GUISprite settingsText;
	GUISprite backText;
	GUISprite container;

	GAMESTATE gameState;

	void HandleInputs();

	void TextPass();
	void SpritePass();


public:

	GAMESTATE GetGameState();

	SettingsUI();
	void Draw();
};

