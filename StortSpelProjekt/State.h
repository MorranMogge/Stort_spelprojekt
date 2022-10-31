#pragma once
#include <iostream>

enum GAMESTATE
{
	MENU,
	SETTINGS,
	CREDITS,
	GAME,
	NOCHANGE,
	EXIT,
	WIN
};

class State
{
public:

	State();
	virtual ~State() {};

	//virtual ~State() = 0;
	virtual GAMESTATE Update() = 0;
	virtual void Render() = 0;
};

