#include "stdafx.h"
#include "Potion.h"
#include "Player.h"

Potion::Potion(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:Item(useMesh, pos, rot, id), playerPtr(nullptr), restoringAmount(10)
{
	
}

Potion::Potion(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:Item(objectPath, pos, rot, id), playerPtr(nullptr), restoringAmount(10)
{

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
