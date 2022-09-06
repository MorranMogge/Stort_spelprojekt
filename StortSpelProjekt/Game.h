#pragma once
#include "State.h"

class Game : public State
{
public:
	Game();
	~Game();

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};