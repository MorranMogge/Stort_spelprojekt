#pragma once
#include <string>

//temp klass eftersom det inte finns en player
class Component
{
private:
	float xPos;
	float yPos;
	float zPos;

	int inUseBy;
public:
	Component();

	//skicka in char 'x', 'y' eller 'z'
	float getposition(char whichPos)const;
	int getInUseById()const;

	void setInUseBy(const int inUseById);
	void setPosition(float pos[3]);
	void setPosition(float x, float y, float z);
};