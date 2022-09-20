#pragma once
#include "State.h"
#include "Credits.h"

class CreditsMenu : public State
{
	Credits ui;

public:
	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

