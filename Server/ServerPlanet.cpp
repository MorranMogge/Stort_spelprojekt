#include "PhysicsWorld.h"
#include "ServerPlanet.h"
#include "DirectXMathHelper.h"

Planet::Planet(const DirectX::SimpleMath::Vector3& scale, const DirectX::XMFLOAT3& pos, const float& gravityFactor)
	:position(pos), rotation(DirectX::XMFLOAT3(0.f, 0.f, 0.f)), scale(scale), rotSpeed(0), gravityFactor(gravityFactor), planetCollisionBox(nullptr)
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
	float sizeCorrector = 1.f;
	reactphysics3d::CollisionShapeName shapeName = reactphysics3d::CollisionShapeName::SPHERE;
	this->planetShape = shape;
	switch (shape)
	{
	case SPHERE:
		shapeName = reactphysics3d::CollisionShapeName::SPHERE;
		break;

	case BOX:
		sizeCorrector = 3.f;
		shapeName = reactphysics3d::CollisionShapeName::BOX;
		break;

	default:
		break;
	}
	this->planetCollisionBox = physWorld->returnAddedPhysComponent(shapeName, this->position, this->scale / sizeCorrector);
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

DirectX::SimpleMath::Vector3 Planet::getPlanetPosition() const
{
	return this->position;
}

void Planet::movePlanet(float offset)
{
	this->position.x += offset;
	this->position.y += offset;
	this->position.z += offset;
	this->planetCollisionBox->setPosition(reactphysics3d::Vector3(position.x, position.y, position.z));
	this->gravField->setCenterpoint(this->position);
}

GravityField* Planet::getClosestField(std::vector<Planet*>& planets, const DirectX::SimpleMath::Vector3& position) const
{
	GravityField* closestField = this->gravField;
	DirectX::SimpleMath::Vector3 fieldOne = position - this->position;
	scalarMultiplicationXMFLOAT3(this->getFieldFactor(), fieldOne);
	for (int i = 1; i < planets.size(); i++)
	{
		DirectX::SimpleMath::Vector3 fieldTwo = position - planets[i]->getPlanetPosition();
		scalarMultiplicationXMFLOAT3(this->getFieldFactor(), fieldTwo);
		if (getLength(fieldOne) > getLength(fieldTwo)) { closestField = planets[i]->getGravityField(); fieldOne = fieldTwo; }
	}

	return closestField;
}

DirectX::SimpleMath::Vector3 Planet::getClosestFieldFactor(std::vector<Planet*>& planets, const DirectX::SimpleMath::Vector3& position) const
{
	GravityField* closestField = this->gravField;
	DirectX::SimpleMath::Vector3 fieldOne = position - planets[0]->getPlanetPosition();
	DirectX::SimpleMath::Vector3 fieldTwo;
	scalarMultiplicationXMFLOAT3(this->getFieldFactor(), fieldOne);
	for (int i = 1; i < planets.size(); i++)
	{
		fieldTwo = position - planets[i]->getPlanetPosition();
		scalarMultiplicationXMFLOAT3(this->getFieldFactor(), fieldTwo);
		if (getLength(fieldOne) > getLength(fieldTwo)) { closestField = planets[i]->getGravityField(); fieldOne = fieldTwo; }
	}

	return closestField->calcGravFactor(position);
}

DirectX::SimpleMath::Vector3 Planet::getBothGravFactor(const Planet& otherPlanet, const DirectX::SimpleMath::Vector3& position) const
{
	DirectX::SimpleMath::Vector3 planetGrav = this->gravField->calcGravFactor(position);
	additionXMFLOAT3(planetGrav, otherPlanet.gravField->calcGravFactor(position));
	return this->gravField->calcGravFactor(planetGrav);
}

DirectX::SimpleMath::Vector3 Planet::getAllGravFactor(const std::vector<Planet*>& planets, const DirectX::SimpleMath::Vector3& position) const
{
	DirectX::SimpleMath::Vector3 planetGrav = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);
	for (int i = 0; i < planets.size(); i++)
	{
		additionXMFLOAT3(planetGrav, planets[i]->gravField->calcGravFactor(position));
	}
	return planetGrav;
}

bool Planet::insideGravityField(const DirectX::SimpleMath::Vector3& position) const
{
	DirectX::SimpleMath::Vector3 gravRange = position;
	subtractionXMFLOAT3(gravRange, this->position);
	return getLength(gravRange) <= (float)(PLANETGRAVSIZE * this->gravityFactor * this->scale.x);
}

PhysicsComponent* Planet::getPlanetCollider() const
{
	return this->planetCollisionBox;
}

float Planet::getSize(int index) const
{
	switch (index)
	{
	case 0:
		return this->scale.x;
		break;
	case 1:
		return this->scale.y;
		break;
	case 2:
		return this->scale.z;
		break;
	default:
		return this->scale.x;
		break;
	}

}

GravityField* Planet::getGravityField() const
{
	return this->gravField;
}