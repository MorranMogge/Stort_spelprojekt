#pragma once
#include "State.h"
#include "WinUI.h"
#include "BasicRenderer.h"

class WinMenu : public State
{
	WinUI ui;
	BasicRenderer basicRenderer;

public:

	WinMenu();
	virtual ~WinMenu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};
