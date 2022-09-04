#pragma once
#include "State.h"

#include "Input.h"
#include "SoundCollection.h"
#include "GuiHandler.h"

class Menu : public State
{
private:

	GuiHandler ui;

public:
	Menu();
	~Menu();

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

