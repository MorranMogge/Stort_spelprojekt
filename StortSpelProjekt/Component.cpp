#include "stdafx.h"
#include "Component.h"

Component::Component(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field)
	:Item(useMesh, pos, rot, id, onlineId, 0, field), pickedUp(false)
{
	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(2, 5), 2);

	//Item Icon
	float constant = 4.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> tempStr{ "icon_component.png", "icon_component2.png" };
	this->itemIcon = new BilboardObject(tempStr, iconPos);
	this->itemIcon->setOffset(constant);
}

Component::Component(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field)
	:Item(objectPath, pos, rot, id, onlineId, 0, field), pickedUp(false)
{
	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(2, 5), 2);

	//Item Icon
	float constant = 4.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> tempStr{ "icon_component.png", "icon_component2.png" };
	this->itemIcon = new BilboardObject(tempStr, iconPos);
	this->itemIcon->setOffset(constant);
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
