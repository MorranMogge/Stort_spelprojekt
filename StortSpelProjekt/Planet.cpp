#include "stdafx.h"
#include "Planet.h"

Planet::Planet(Mesh* useMesh, const float& gravityFactor, const DirectX::XMFLOAT3& pos, const DirectX::SimpleMath::Vector3& scale)
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
	reactphysics3d::CollisionShapeName shapeName;
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
}

float Planet::getFieldFactor() const
{
	switch (planetShape)
	{
	case SPHERE:
		return this->gravityFactor * 10.0f * this->scale.x;
		break;
	
	case BOX:
		return this->gravityFactor * 10.0f * this->scale.x;
		break;
	
	default:
		return this->gravityFactor * 10.0f * this->scale.x;
		break;
	}
}

GravityField* Planet::getClosestField(const Planet& otherPlanet, const DirectX::SimpleMath::Vector3& position) const
{
	GravityField* closestField = nullptr;
	if (this->getFieldFactor() > otherPlanet.getFieldFactor()) closestField = this->gravField;
	else closestField = otherPlanet.getGravityField();
	return closestField;
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
