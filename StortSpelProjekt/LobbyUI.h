#pragma once

#include "GUISprite.h"
#include "State.h"
#include <GamePad.h>

class LobbyUI
{
	GUISprite Loading;
	GUISprite ready_b, ready2_b;
	GUISprite ready_r, ready2_r;

	GUISprite checkbox_false, checkbox_true;

	GUISprite player1, player2, player3, player4;
	GUISprite player1_checkbox_false, player2_checkbox_false, player3_checkbox_false, player4_checkbox_false;
	GUISprite player1_checkbox_true, player2_checkbox_true, player3_checkbox_true, player4_checkbox_true;

	GUISprite player1_checkbox_false_r, player2_checkbox_false_r, player3_checkbox_false_r, player4_checkbox_false_r;
	GUISprite player1_checkbox_true_r, player2_checkbox_true_r, player3_checkbox_true_r, player4_checkbox_true_r;

	GUISprite readyText1, readyText2, readyText3, readyText4;
	GUISprite connectedPlayersText;
	GUISprite n0, n1, n2, n3, n4, n5, n6, n7, n8, n9;

	bool hit_ready = false;

	void SpritePass();

public:
	int connectedPlayers = 69;
	bool ready = false;
	bool isLoading = false;

	bool p1 = false;
	bool p2 = false;
	bool p3 = false;
	bool p4 = false;

	bool p1_connect = false;
	bool p2_connect = false;
	bool p3_connect = false;
	bool p4_connect = false;

	LobbyUI();
	~LobbyUI();

	void Draw();
	void DrawLoading();
};