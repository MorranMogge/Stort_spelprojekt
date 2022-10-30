#include "stdafx.h"
#include "SpaceShip.h"

using namespace DirectX;

SpaceShip::SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, const DirectX::XMFLOAT3& scale, const int& nrofComp)
	:GameObject(useMesh, pos, DirectX::XMFLOAT3(0,0,0), id, field, scale), compToComplete(nrofComp)
{
	//Set rotation to gravity field
	this->setRot(this->getRotOrientedToGrav());

	//Icon initiation
	float constant = 65.0f;
	std::vector<std::string> filenames{ "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 test(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	this->rocketStatusQuad = new BilboardObject(filenames, test);
	this->rocketStatusQuad->setOffset(constant);

	//Team switch
	switch (team)
	{
	case 0: 
		red = this; mesh->matKey[0] = "spaceshipTexture1.jpg"; break;
	case 1: 
		blue = this; break;
	}
}

SpaceShip::SpaceShip(const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, const DirectX::XMFLOAT3& scale, const int& nrofComp)
	:GameObject("../Meshes/rocket", pos, DirectX::XMFLOAT3(0, 0, 0), id, field, scale), compToComplete(nrofComp)
{
	//Set rotation to gravity field
	this->setRot(this->getRotOrientedToGrav());

	//Icon initiation
	float constant = 65.0f;
	std::vector<std::string> filenames{ "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 test(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	this->rocketStatusQuad = new BilboardObject(filenames, test);
	this->rocketStatusQuad->setOffset(constant);

	//Team switch
	switch (team)
	{
	case 0:
		red = this; mesh->matKey[0] = "spaceshipTexture1.jpg"; break;

	case 1:
		blue = this; break;
	}
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

int SpaceShip::getNrOfComponents()
{
	 return (int)components.size(); 
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

void SpaceShip::drawQuad()
{
	rocketStatusQuad->bindAndDraw((int)this->components.size(), 0);//Changes texture depending on components
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

void SpaceShip::move(const DirectX::XMFLOAT3& grav, const float& deltaTime)
{
	upVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 0.0f);
	this->position += upVector * 18.f * deltaTime;
}