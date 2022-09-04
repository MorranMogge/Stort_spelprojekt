#pragma once
#include "GUISprite.h"
#include "GUIText.h"

class GuiHandler final
{
	GUISprite spriteWeaponBox;
	GUIText testText;

	void TextPass();
	void SpritePass();

public:

	GuiHandler();
	void Draw();

};

