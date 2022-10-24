#include "stdafx.h"
#include "SpaceShip.h"

SpaceShip::SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale, const int& nrofComp)
	:GameObject(useMesh, pos, rot, id, scale), nrOfComponents(nrofComp)
{

}

SpaceShip::SpaceShip(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale, const int& nrofComp)
	:GameObject("../Meshes/rocket", pos, rot, id, scale), nrOfComponents(nrofComp)
{
	
}

SpaceShip::~SpaceShip()
{
	//Delete components?
	for (int i = 0; i < this->components.size(); i++)
	{
		delete this->components.at(i);
	}
}

bool SpaceShip::detectedComponent(GameObject* objectToCheck)
{
	bool didDetect = false;

	if (this->withinRadious(objectToCheck, 8.0f))
	{
		didDetect = true;
	}
	return didDetect;
}

bool SpaceShip::detectedComponent(Component* componentToCheck)
{
	bool didDetect = false;

	if (this->withinRadious(componentToCheck, 8.0f))
	{
		didDetect = true;
	}
	return didDetect;
}

void SpaceShip::update()
{
}
