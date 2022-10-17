#pragma once
#include "State.h"
#include "SettingsUI.h"
#include "BasicRenderer.h"

class SettingsMenu : public State
{
	SettingsUI ui;

	BasicRenderer basicRenderer;
public:

	SettingsMenu();
	virtual ~SettingsMenu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};
