#include "Component.h"

Component::Component()
{
	xPos = 20.0f;
	yPos = 30.0f;
	zPos = 40.0f;
	inUseBy = -1;
}

float Component::getposition(char whichPos) const
{
	switch (whichPos)
	{
	case('x'):
		//return xPos;
		return this->physComp->getPosV3().x;
		break;
	case('y'):
		//return yPos;
		return this->physComp->getPosV3().y;
		break;
	case('z'):
		//return zPos;
		return this->physComp->getPosV3().z;
		break;
	default:
		return 69.0f;
		break;
	}
}

int Component::getInUseById() const
{
	return this->inUseBy;
}

PhysicsComponent* Component::getPhysicsComponent() const
{
	return this->physComp;
}

void Component::setPhysCompServer(PhysicsComponent* physComp)
{
	this->physComp = physComp;
}

void Component::setInUseBy(const int inUseById)
{
	this->inUseBy = inUseById;
}

void Component::setPosition(float pos[3])
{
	//this->physComp->setPosition(reactphysics3d::Vector3(pos[0], pos[1], pos[2]));
	this->xPos = pos[0];
	this->yPos = pos[1];
	this->zPos = pos[2];
}

void Component::setPosition(float x, float y, float z)
{
	//this->physComp->setPosition(reactphysics3d::Vector3(x, y, z));
	this->xPos = x;
	this->yPos = y;
	this->zPos = z;
}
