#pragma once
#include "State.h"
#include "Credits.h"
#include "BasicRenderer.h"

class CreditsMenu : public State
{
	Credits ui;

	BasicRenderer basicRenderer;

public:

	CreditsMenu();

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

