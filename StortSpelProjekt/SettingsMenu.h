#pragma once
#include "State.h"
#include "SettingsUI.h"

class SettingsMenu : public State
{
	SettingsUI ui;
public:
	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

