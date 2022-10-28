#pragma once
#include "PhysicsComponent.h"
#include <string>

//temp klass eftersom det inte finns en player
class Component
{
private:
	float xPos;
	float yPos;
	float zPos;
	PhysicsComponent* physComp;
	long onlineId;

	int inUseBy;
public:
	Component();

	//skicka in char 'x', 'y' eller 'z'
	float getposition(char whichPos)const;
	int getInUseById()const;
	long getOnlineId()const;
	
	PhysicsComponent* getPhysicsComponent()const;

	void setOnlineId(long id);
	void setPhysCompServer(PhysicsComponent* physComp);
	void setInUseBy(const int inUseById);
	void setPosition(float pos[3]);
	void setPosition(float x, float y, float z);
};