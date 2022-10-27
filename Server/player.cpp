
#include "player.h"

player::player()
{
	xPos = 20.0f;
	yPos = 30.0f;
	zPos = 40.0f;
	this->dead = false;
}

float player::getposition(char whichPos) const
{
	switch (whichPos)
	{
	case('x'):
		return xPos;
		break;
	case('y'):
		return yPos;
		break;
	case('z'):
		return zPos;
		break;
	default:
		return 69.0f;
		break;
	}
}

void player::playerGotHit(const reactphysics3d::Vector3& force)
{
	this->dead = true;
	this->physComp->setType(reactphysics3d::BodyType::DYNAMIC);
	this->physComp->applyForceToCenter(force);
	this->physComp->applyWorldTorque(force);
	timer.resetStartTime();
}

bool player::getDeathState()
{
	if (timer.getTimePassed(5.0f)) this->dead = true;
	return this->dead;
}

void player::setPhysicsComponent(PhysicsComponent* physComp)
{
	this->physComp = physComp;
}

PhysicsComponent* player::getPhysComp() const
{
	return this->physComp;
}

void player::setMatrix(DirectX::XMFLOAT4X4 matrix)
{
	this->matrix = matrix;
	DirectX::SimpleMath::Quaternion quat;
	quat = DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&matrix));
	this->physComp->setRotation(reactphysics3d::Quaternion(quat.x, quat.y, quat.z, quat.w));
}

DirectX::XMFLOAT4X4 player::getMatrix() const
{ 
	return this->matrix;
}

void player::setPosition(float pos[3])
{
	this->xPos = pos[0];
	this->yPos = pos[1];
	this->zPos = pos[2];
	this->physComp->setPosition(reactphysics3d::Vector3(pos[0], pos[1], pos[2]));
}

void player::setPosition(float x, float y, float z)
{
	this->matrix._14 = x;
	this->matrix._24 = y;
	this->matrix._34 = z;
	this->xPos = x;
	this->yPos = y;
	this->zPos = z;
	this->physComp->setPosition(reactphysics3d::Vector3(x, y, z));
}
