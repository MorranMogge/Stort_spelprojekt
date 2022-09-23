#include "Potion.h"

Potion::Potion(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:Item(useMesh, pos, rot, id)
{
}

Potion::Potion(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:Item(objectPath, pos, rot, id)
{

}

Potion::~Potion()
{
}

void Potion::useItem()
{
	this->setPos({ -100, -100, -100 });
	
}
