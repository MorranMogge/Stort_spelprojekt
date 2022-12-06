#pragma once
#include <string>
#include <DirectXMath.h>
#include "PhysicsComponent.h"
#include "TimeStruct.h"

//temp klass eftersom det inte finns en player
class Player
{
private:
	int onlineId;
	int readyStatus;
	float xPos;
	float yPos;
	float zPos;
	DirectX::XMFLOAT4X4 matrix;
	PhysicsComponent* physComp;
	bool dead;
	TimeStruct timer;

	int animID = 0;
	float animSpeed = 1;

public:
	Player();

	//skicka in char 'x', 'y' eller 'z'
	float getposition(char whichPos)const;

	void playerGotHit(const reactphysics3d::Vector3& force);
	bool getDeathState();
	void setPhysicsComponent(PhysicsComponent* physComp);
	PhysicsComponent* getPhysComp()const;
	void setMatrix(DirectX::XMFLOAT4X4 matrix);
	void setOnlineId(int nr);
	
	//ready status 0 is not ready, status 1 is ready 
	void setReadyStatus(int status);
	int getReadyStatus()const;
	int getOnlineID()const;
	DirectX::XMFLOAT4X4 getMatrix()const;
	void setPosition(float pos[3]);
	void setPosition(float x, float y, float z);
	void updatePosViaPhysComp();

	void setAnimData(int animId, float animSpeed);
	void getAnimData(int& animId, float& animSpeed);
};