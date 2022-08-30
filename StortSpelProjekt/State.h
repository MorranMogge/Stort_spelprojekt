#pragma once
#include <iostream>

enum GAMESTATE
{
	MENU,
	GAME,
	NOCHANGE,
	EXIT
};

class State
{
public:
	State();

	//virtual ~State() = 0;
	virtual GAMESTATE Update() = 0;
	virtual void Render() = 0;
};

