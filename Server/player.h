#pragma once
#include <string>

//temp klass eftersom det inte finns en player
class player
{
private:
	float xPos;
	float yPos;
	float zPos;
public:
	player();

	//skicka in char 'x', 'y' eller 'z'
	float getposition(char whichPos)const;

	void setPosition(float pos[3]);
};