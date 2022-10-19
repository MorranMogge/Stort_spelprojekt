#include "stdafx.h"
#include "Item.h"

Item::Item(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:GameObject(useMesh, pos, rot, id), pickedUp(false), itemIcon(nullptr), particles(nullptr)
{
}

Item::Item(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:GameObject(objectPath, pos, rot, id), pickedUp(false), itemIcon(nullptr), particles(nullptr)
{
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

void Item::throwItem()
{
}
