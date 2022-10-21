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

int Component::getInUseById() const
{
	return this->inUseBy;
}

void Component::setInUseBy(const int inUseById)
{
	this->inUseBy = inUseById;
}

void Component::setPosition(float pos[3])
{
	this->xPos = pos[0];
	this->yPos = pos[1];
	this->zPos = pos[2];
}

void Component::setPosition(float x, float y, float z)
{
	this->xPos = x;
	this->yPos = y;
	this->zPos = z;
}
