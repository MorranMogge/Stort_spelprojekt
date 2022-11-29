#pragma once
#include <string>
#include <DirectXMath.h>
#include "PhysicsComponent.h"
#include "TimeStruct.h"

//temp klass eftersom det inte finns en player
class Player
{
private:
	float xPos;
	float yPos;
	float zPos;
	DirectX::XMFLOAT4X4 matrix;
	PhysicsComponent* physComp;
	bool dead;
	TimeStruct timer;

public:
	Player();

	//skicka in char 'x', 'y' eller 'z'
	float getposition(char whichPos)const;

	void playerGotHit(const reactphysics3d::Vector3& force);
	bool getDeathState();
	void setPhysicsComponent(PhysicsComponent* physComp);
	PhysicsComponent* getPhysComp()const;
	void setMatrix(DirectX::XMFLOAT4X4 matrix);
	DirectX::XMFLOAT4X4 getMatrix()const;
	void setPosition(float pos[3]);
	void setPosition(float x, float y, float z);
	void updatePosViaPhysComp();
};