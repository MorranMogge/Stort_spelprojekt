#include "stdafx.h"
#include "Item.h"

Item::Item(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, GravityField* field)
	:GameObject(useMesh, pos, rot, id, field), pickedUp(false), itemIcon(nullptr), particles(nullptr)
{
	//Initilize timer
	tStruct.startTime;
}

Item::Item(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, GravityField* field)
	:GameObject(objectPath, pos, rot, id, field), pickedUp(false), itemIcon(nullptr), particles(nullptr)
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
		this->itemIcon->bindAndDraw(0, 0);
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

void Item::throwItem()
{
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

