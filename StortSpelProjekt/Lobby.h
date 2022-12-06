#pragma once
#include "Client.h"

#pragma once
#include "State.h"
#include "BasicRenderer.h"
#include "LobbyUI.h"
#include "Client.h"

class Lobby : public State
{
	LobbyUI ui;
	BasicRenderer basicRenderer;


public:

	Lobby();
	virtual ~Lobby() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};