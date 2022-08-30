#pragma once
#include "State.h"

class Game : public State
{
public:
	Game();
	~Game();

	// Inherited via State
	virtual void update() override;
	virtual void draw() override;
};

