#pragma once

#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"
#include "ImGuiHelper.h"

class SettingsUI final
{

	GUISprite settingsText;
	GUISprite backText;

	GUISprite directxText,vulkanText;
	GUISprite directxText2,vulkanText2;

	GUISprite checkbox_false,checkbox_true;

	GUISprite text720p,text1080p;
	GUISprite text720p2,text1080p2;

	GUISprite dropDown,dropDown2;

	GUISprite dropDown_menu1,dropDown_menu2;
	GUISprite dropDown_menu3,dropDown_menu4;

	GUISprite fullscreenText, graphicAPItext, resulotionText;

	ImGuiHelper imGui;

	GAMESTATE gameState;

	void HandleInputs();
	void SpritePass();
	void Init();

public:

	bool fullscreen = false;
	bool dropdown1 = false;
	bool dropdown2 = false;

	bool isDirectx = true;
	inline static bool is720p = true;

	GAMESTATE GetGameState();

	SettingsUI();
	~SettingsUI();

	void Draw();
};

