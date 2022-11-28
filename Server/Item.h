#pragma once
#include "PhysicsComponent.h"
#include "player.h"
class Item
{
private:
	// THESE POSITIONS ARENT UPDATED IN getPosXMFLOAT3(), BUT WORK IN getposition()
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

	//set in use by equal to -1 if the item isnt in use
	void setInUseBy(const int nr);

	void setOnlineId(int Id);
	void setOnlineType(int type);

	float getposition(char whichPos);
	DirectX::XMFLOAT3 getPosXMFLOAT3()const;

	void setPosition(float pos[3]);
	void setPosition(float x, float y, float z);

	bool getActiveState()const;
	void setActiceSate(const bool status);

	int getInUseById()const;

	virtual void use(const player* playerHoldingItem) = 0;
};