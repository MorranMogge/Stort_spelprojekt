#pragma once
#include "State.h"

#include "Input.h"
#include "SoundCollection.h"

class Menu : public State
{
private:



public:
	Menu();
	~Menu();
	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

