#pragma once
#include <iostream>

class State
{
public:
	State();

	virtual ~State() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
};

