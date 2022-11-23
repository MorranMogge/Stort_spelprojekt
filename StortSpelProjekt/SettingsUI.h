#pragma once

#include "GUISprite.h"
#include "GUIText.h"
#include "State.h"
#include "ImGuiHelper.h"
#include <GamePad.h>

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

	std::unique_ptr<DirectX::GamePad> gamePad;

	GAMESTATE gameState;

	int selected = 0;
	bool any_DropDown_Active = false;
	int selected_dropdown = 0;

	bool ATrigged = false;
	bool BTrigged = false;
	bool upTrigged = false;
	bool downTrigged = false;

	void DoFullScreen();

	void HandleInputs();
	void SpritePass();

public:

	inline static bool fullscreen = false;
	inline static bool dropdown1 = false;
	inline static bool dropdown2 = false;

	inline static bool isDirectx = true;
	inline static bool is720p = true;

	GAMESTATE GetGameState();

	SettingsUI();
	~SettingsUI();

	void Draw();
};

