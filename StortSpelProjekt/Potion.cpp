#include "stdafx.h"
#include "Potion.h"
#include "Player.h"

Potion::Potion(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field)
	:Item(useMesh, pos, rot, id, field), playerPtr(nullptr), restoringAmount(10)
{
	this->itemIcon = new BilboardObject("p1.png", this->position);
}

Potion::Potion(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field)
	:Item(objectPath, pos, rot, id, field), playerPtr(nullptr), restoringAmount(10)
{
	float constant = 15.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 billPos(upDir.x* constant, upDir.y* constant, upDir.z* constant);
	this->itemIcon = new BilboardObject("p1.png", billPos);
}

Potion::~Potion()
{
}

void Potion::useItem()
{
	this->playerPtr->addHealth(this->restoringAmount);
	this->setPos({ -100, -100, -100 });
	
}

int Potion::getRestore() const
{
	return this->restoringAmount;
}

void Potion::setPlayerptr(Player* ptr)
{
	if (!playerPtr)
		this->playerPtr = ptr;
}
