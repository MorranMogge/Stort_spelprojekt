#pragma once
#include "State.h"
#include "MenuUI.h"
#include "BasicRenderer.h"
#include "SoundCollection.h"

class Menu : public State
{
private:

	MenuUI ui;
	BasicRenderer basicRenderer;
	Sound menuMusic;

public:
	Menu();
	virtual ~Menu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

