#pragma once

#include "State.h"
#include "BasicRenderer.h"
#include "InputIPUI.h"


class InputIP : public State
{
private:
	InputIPUI ui;
	BasicRenderer basicRenderer;

public:

	InputIP();
	virtual ~InputIP() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};