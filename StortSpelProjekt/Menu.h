#pragma once
#define NOMINMAX
#include "State.h"
#include "GuiHandler.h"
#include "BasicRenderer.h"


class Menu : public State
{
private:

	GuiHandler ui;
	BasicRenderer basicRenderer;

public:
	Menu();
	virtual ~Menu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

