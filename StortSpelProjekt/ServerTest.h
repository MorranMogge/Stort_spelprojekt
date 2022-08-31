#pragma once
#include "State.h"

class ServerTest : public State
{
public:
	ServerTest();
	~ServerTest();

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};
