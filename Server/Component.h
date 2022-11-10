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


	int onlineId;
	int inUseBy;
	bool active;
public:
	Component();

	//skicka in char 'x', 'y' eller 'z'
	float getposition(char whichPos)const;
	DirectX::XMFLOAT3 getPosXMFLOAT3()const;
	int getInUseById()const;
	
	PhysicsComponent* getPhysicsComponent()const;
	bool getActiveState()const;

	void setInactive();
	void setPhysCompServer(PhysicsComponent* physComp);
	void setInUseBy(const int inUseById);
	void setPosition(float pos[3]);
	void setPosition(float x, float y, float z);
};