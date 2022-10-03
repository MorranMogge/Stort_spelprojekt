#pragma once
#include "GUISprite.h"

class Credits final
{

	GUISprite creditsText;
	GUISprite nameText;
	GUISprite backText;

	void HandleInputs();

	void TextPass();
	void SpritePass();

public:
	inline static bool show = false;
	Credits();
	void Draw();
};

