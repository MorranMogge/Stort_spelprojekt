#include "stdafx.h"
#include "SpaceShip.h"

SpaceShip::SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const DirectX::XMFLOAT3& direction, const int& id, const DirectX::XMFLOAT3& scale, const int& nrofComp)
	:GameObject(useMesh, pos, rot, id, scale), nrOfComponents(nrofComp)
{
	//Bilboard test
	int constant = 18;
	DirectX::XMFLOAT3 test(-direction.x * constant, -direction.y * constant, -direction.z * constant);
	std::vector<std::string> filenames{ "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };
	this->rocketStatusQuad = new BilboardObject(filenames, test);
}

SpaceShip::SpaceShip(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const DirectX::XMFLOAT3& direction, const int& id, const DirectX::XMFLOAT3& scale, const int& nrofComp)
	:GameObject("../Meshes/rocket", pos, rot, id, scale), nrOfComponents(nrofComp)
{
	//Bilboard test
	int constant = 18;
	DirectX::XMFLOAT3 test(-direction.x * constant, -direction.y * constant, -direction.z * constant);
	std::vector<std::string> filenames{ "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };
	this->rocketStatusQuad = new BilboardObject(filenames, test);
}

SpaceShip::~SpaceShip()
{
	//Delete components?
	for (int i = 0; i < this->components.size(); i++)
	{
		delete this->components.at(i);
	}
	delete this->rocketStatusQuad;
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

void SpaceShip::update()
{
}

void SpaceShip::drawQuad()
{
	rocketStatusQuad->bindAndDraw(this->components.size(), 0);//Changes texture depending on components
}

bool SpaceShip::isFinished()
{
	bool complete = false;
	if (components.size() >= compToComplete)
	{
		complete = true;
	}
	return complete;
}
