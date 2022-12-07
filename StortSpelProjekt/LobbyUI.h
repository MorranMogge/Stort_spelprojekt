#pragma once

#include "GUISprite.h"
#include "State.h"
#include <GamePad.h>

class LobbyUI
{
	GUISprite Loading;

	void SpritePass();
public:

	LobbyUI();
	~LobbyUI();

	void Draw();
};