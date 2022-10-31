#pragma once
#include "State.h"
#include "LoseUI.h"
#include "BasicRenderer.h"

class LoseMenu : public State
{
	LoseUI ui;
	BasicRenderer basicRenderer;

public:

	LoseMenu();
	virtual ~LoseMenu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};
