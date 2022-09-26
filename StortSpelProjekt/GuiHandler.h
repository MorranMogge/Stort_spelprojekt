#pragma once
#include "GUISprite.h"
#include "GUIText.h"

class GuiHandler final
{
	GUISprite spriteWeaponBox;

	GUISprite start;
	GUISprite settings;
	GUISprite credits;
	GUISprite exit;


	GUIText testText;

	void HandleInputs();

	void TextPass();
	void SpritePass();

public:

	inline static bool show = true;
	inline static bool isExit = false;
	GuiHandler();
	void Draw();

};
