#pragma once

#include "GUISprite.h"
#include "GUIText.h"

class SettingsUI final
{

	GUISprite settingsText;
	GUISprite backText;
	GUISprite container;

	void HandleInputs();

	void TextPass();
	void SpritePass();

public:
	inline static bool show = false;
	SettingsUI();
	void Draw();
};

