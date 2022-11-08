#include "Item.h"

Item::Item()
{
	this->onlineId = -1;
	this->onlineType = -1;
	this->inUseBy = -1;
	xPos = 20.0f;
	yPos = 30.0f;
	zPos = 40.0f;
}

Item::Item(int id, int type)
{
	xPos = 20.0f;
	yPos = 30.0f;
	zPos = 40.0f;
	this->inUseBy = -1;
	this->physComp = nullptr;
	this->onlineId = id;
	this->onlineType = type;
}

Item::~Item()
{

}

void Item::setPhysCompServer(PhysicsComponent* physComp)
{
	this->physComp = physComp;
}

PhysicsComponent* Item::getPhysicsComponent() const
{
	return this->physComp;
}

int Item::getOnlineId()
{
	return this->onlineId;
}

int Item::getOnlineType()
{
	return onlineType;
}

void Item::setInUseBy(const int nr)
{
	this->inUseBy = nr;
}

void Item::setOnlineId(int Id)
{
	this->onlineId = Id;
}

void Item::setOnlineType(int type)
{
	this->onlineType = type;
}

float Item::getposition(char whichPos)
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

DirectX::XMFLOAT3 Item::getPosXMFLOAT3()const
{
	return DirectX::XMFLOAT3(xPos, yPos, zPos);
}

void Item::setPosition(float pos[3])
{
	this->physComp->setPosition(reactphysics3d::Vector3(pos[0], pos[1], pos[2]));
	this->xPos = pos[0];
	this->yPos = pos[1];
	this->zPos = pos[2];
}

void Item::setPosition(float x, float y, float z)
{
	this->physComp->setPosition(reactphysics3d::Vector3(x, y, z));
	this->xPos = x;
	this->yPos = y;
	this->zPos = z;
}

bool Item::getActiveState() const
{
	return this->active;
}

void Item::setActiceSate(const bool status)
{
	this->active = status;
}

int Item::getInUseById()const
{
	return this->inUseBy;
}

