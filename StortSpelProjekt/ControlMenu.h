#pragma once

#include "State.h"
#include "ControlUI.h"
#include "BasicRenderer.h"

class ControlMenu : public State
{
	ControlUI ui;
	BasicRenderer basicRenderer;

public:

	ControlMenu();
	virtual ~ControlMenu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};
