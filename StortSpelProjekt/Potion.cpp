#include "Potion.h"
#include "Player.h"

Potion::Potion(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:Item(useMesh, pos, rot, id), playerPtr(nullptr), restoringAmount(10)
{
	
}

Potion::Potion(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
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
