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
	GUISprite connectedPlayersText;
	GUISprite n0, n1, n2, n3, n4, n5, n6, n7, n8, n9;

	bool hit_ready = false;

	void SpritePass();
public:
	int connectedPlayers = 69;
	bool ready = false;
	bool isLoading = false;

	LobbyUI();
	~LobbyUI();

	void Draw();
};