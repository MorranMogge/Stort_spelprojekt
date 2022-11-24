#include "stdafx.h"
#include "Item.h"


Item::Item(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, const int& onlineType, GravityField* field)
	:GameObject(useMesh, pos, rot, id, field), pickedUp(false), itemIcon(nullptr), particles(nullptr), withinPlayerReach(false), onlineId(onlineId),onlineType(onlineType)
{
	//Initilize timer
	tStruct.startTime;
}


Item::Item(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, const int& onlineType, GravityField* field)
	:GameObject(objectPath, pos, rot, id, field), pickedUp(false), itemIcon(nullptr), particles(nullptr), withinPlayerReach(false), onlineId(onlineId), onlineType(onlineType)
{
	//Initilize timer
	tStruct.startTime;
}

Item::~Item()
{
	if (this->itemIcon != nullptr)
	{
		delete this->itemIcon;
	}
	if (this->particles != nullptr)
	{
		delete this->particles;
	}
}



void Item::drawIcon()
{
	if (this->itemIcon != nullptr && !pickedUp)
	{
		if (withinPlayerReach)
		{
			this->itemIcon->bindAndDraw(1, 0);
		}
		else
		{
			this->itemIcon->bindAndDraw(0, 0);
		}
	}
}

void Item::setPickedUp(bool pickedUp)
{
	this->pickedUp = pickedUp;
}

void Item::drawParticles()
{
	if (this->particles != nullptr && pickedUp)
	{
		if (tStruct.getDt() < 1)
		{
			this->particles->BindAndDraw(0);
		}
	}
}

void Item::checkDistance(GameObject* playerToCheck)
{
	if (this->withinRadious(playerToCheck, 5.5f))
	{
		this->withinPlayerReach = true;
	}
	else
	{
		this->withinPlayerReach = false;
	}
}

void Item::throwItem()
{
}

int Item::getOnlineType() const
{
	return onlineType;
}

int Item::getOnlineId() const
{
	return onlineId;
}

void Item::update()
{
	//Update movement
	this->updateRotation();

	//Update icon movement
	if (this->itemIcon != nullptr)
	{
		float constant = itemIcon->getOffset();
		DirectX::XMFLOAT3 upDir = this->getUpDirection();
		DirectX::XMFLOAT3 itemPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);		
		this->itemIcon->setPosition(this->position + itemPos);
	}

	//Update particle movement
	if (this->particles != nullptr && !this->pickedUp)
	{
		tStruct.resetStartTime();
		DirectX::XMFLOAT3 rot= this->getRotOrientedToGrav();
		this->particles->setPosition(this->position);
		this->particles->setRotation(this->getUpDirection());
		this->particles->updateBuffer();
	}
}

