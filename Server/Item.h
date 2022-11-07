#pragma once
#include "PhysicsComponent.h"

class Item
{
private:
	float xPos;
	float yPos;
	float zPos;

	PhysicsComponent* physComp;

	bool active;
	int inUseBy;
	int onlineType;
	int onlineId;
public:
	Item();
	Item(int Id, int type);
	~Item();

	void setPhysCompServer(PhysicsComponent* physComp); //FLYTTA TILL ITEM.Hs

	PhysicsComponent* getPhysicsComponent()const;

	int getOnlineId();
	int getOnlineType();

	void setOnlineId(int Id);
	void setOnlineType(int type);

	float getposition(char whichPos);
	DirectX::XMFLOAT3 getPosXMFLOAT3()const;

	void setPosition(float pos[3]);
	void setPosition(float x, float y, float z);

	bool getActiveState()const;
	void setActiceSate(const bool status);

	int getInUseById()const;

	virtual void use() = 0;
};