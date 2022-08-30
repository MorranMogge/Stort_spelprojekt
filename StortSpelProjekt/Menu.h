#pragma once
#include "State.h"

class Menu : public State
{
private:

public:
	Menu();
	~Menu();
	// Inherited via State
	virtual void update() override;
	virtual void draw() override;
};

