#include "stdafx.h"
#include "Planet.h"
#include "DirectXMathHelper.h"

Planet::Planet(Mesh* useMesh, const DirectX::SimpleMath::Vector3& scale, const DirectX::XMFLOAT3& pos, const float& gravityFactor)
	:mesh(useMesh), position(pos), rotation(DirectX::XMFLOAT3(0.f,0.f,0.f)), scale(scale), rotSpeed(0), gravityFactor(gravityFactor), planetCollisionBox(nullptr)
{
	this->planetShape = NONDISCLOSEDSHAPE;
	this->gravField = new GravityField(gravityFactor, pos, scale.x);
}

Planet::~Planet()
{
	delete this->gravField;
}

void Planet::setPlanetShape(PhysicsWorld* physWorld, const PlanetShape& shape)
{
	reactphysics3d::CollisionShapeName shapeName = reactphysics3d::CollisionShapeName::SPHERE;
	this->planetShape = shape;
	switch (shape)
	{
	case SPHERE:
		shapeName = reactphysics3d::CollisionShapeName::SPHERE;
		break;

	case BOX:
		shapeName = reactphysics3d::CollisionShapeName::BOX;
		break;

	default:
		break;
	}
	this->planetCollisionBox = physWorld->returnAddedPhysComponent(shapeName, this->position, this->scale);
	this->planetCollisionBox->setType(reactphysics3d::BodyType::STATIC);
}

float Planet::getFieldFactor() const
{
	switch (planetShape)
	{
	case SPHERE:
		return this->gravityFactor * PLANETGRAVSIZE * this->scale.x;
		break;
	
	case BOX:
		return this->gravityFactor * PLANETGRAVSIZE * this->scale.x;
		break;
	
	default:
		return this->gravityFactor * PLANETGRAVSIZE * this->scale.x;
		break;
	}
}

GravityField* Planet::getClosestField(const Planet& otherPlanet, const DirectX::SimpleMath::Vector3& position) const
{
	GravityField* closestField = nullptr;
	DirectX::SimpleMath::Vector3 fieldOne;
	DirectX::SimpleMath::Vector3 fieldTwo;
	scalarMultiplicationXMFLOAT3(this->getFieldFactor(), fieldOne);
	scalarMultiplicationXMFLOAT3(otherPlanet.getFieldFactor(), fieldTwo);
	if (getLength(fieldOne) > getLength(fieldTwo)) closestField = this->gravField;
	else closestField = otherPlanet.getGravityField();
	return closestField;
}

DirectX::SimpleMath::Vector3 Planet::getBothGravFactor(const Planet& otherPlanet, const DirectX::SimpleMath::Vector3& position) const
{
	DirectX::SimpleMath::Vector3 planetGrav = this->gravField->calcGravFactor(position);
	additionXMFLOAT3(planetGrav, otherPlanet.gravField->calcGravFactor(position));
	return this->gravField->calcGravFactor(planetGrav);
}

DirectX::SimpleMath::Vector3 Planet::getAllGravFactor(const std::vector<Planet *>& planets, const DirectX::SimpleMath::Vector3& position) const
{
	DirectX::SimpleMath::Vector3 planetGrav = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);
	for (int i = 0; i < planets.size(); i++)
	{
		additionXMFLOAT3(planetGrav, planets[i]->gravField->calcGravFactor(position));
	}
	return this->gravField->calcGravFactor(planetGrav);
}

bool Planet::insideGravityField(const DirectX::SimpleMath::Vector3& position) const
{
	DirectX::SimpleMath::Vector3 gravRange = position;
	subtractionXMFLOAT3(gravRange, this->position);
	return getLength(gravRange) <= (float)(PLANETGRAVSIZE * this->gravityFactor * this->scale.x);
}

GravityField* Planet::getGravityField() const
{
	return this->gravField;
}

void Planet::drawPlanet()
{
	this->mesh->position = this->position;
	this->mesh->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
	this->mesh->scale = scale;

	this->mesh->UpdateCB();
	this->mesh->DrawWithMat();
}
