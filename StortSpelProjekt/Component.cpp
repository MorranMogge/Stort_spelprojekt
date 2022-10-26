#include "stdafx.h"
#include "Component.h"

Component::Component(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId)
	:Item(objectPath, pos, rot, id, onlineId, 0), pickedUp(false)
{
}

Component::~Component()
{
}

void Component::interactWithComponent()
{
	this->pickedUp = !this->pickedUp;
}

bool Component::returnStatus() const
{
	return this->pickedUp;
}

void Component::useItem()
{
	std::cout << "You used the Component\n";
}
