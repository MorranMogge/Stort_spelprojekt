#include "stdafx.h"
#include "Planet.h"
#include "DirectXMathHelper.h"
#include "PhysicsWorld.h"
#include <complex>

Planet::Planet(Mesh* useMesh, const DirectX::SimpleMath::Vector3& scale, const DirectX::XMFLOAT3& pos, const float& gravityFactor, Mesh* atmoMesh, const DirectX::SimpleMath::Vector3& atmoColor, const float & atmoDensity)
	:mesh(useMesh), position(pos), rotation(DirectX::XMFLOAT3(0.f,0.f,0.f)), scale(scale), rotSpeed(DirectX::SimpleMath::Vector3(0,0,0)), gravityFactor(gravityFactor), planetCollisionBox(nullptr), rotDegrees(0)
{
	this->planetShape = NONDISCLOSEDSHAPE;
	this->gravField = new GravityField(gravityFactor, pos, scale.x);
	this->originPoint = pos;
	
	//Set atmosphere mesh
	this->atmosphere = atmoMesh;

	
	if (atmoMesh != nullptr)
	{
		//Set up color buffer
		this->colorBuffer.Initialize(GPU::device, GPU::immediateContext);
		this->colorBuffer.getData() = DirectX::XMFLOAT4(atmoColor.x, atmoColor.y, atmoColor.z, atmoDensity);
		this->colorBuffer.applyData();
	
		//Set atmosphere properties 
		this->atmosphere->UpdateCB(pos, DirectX::XMMatrixIdentity(), this->scale + DirectX::XMFLOAT3(10, 10, 10));
	}
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
	this->planetCollisionBox = physWorld->returnAddedPhysComponent(shapeName, this->position, this->scale/sizeCorrector);
	this->planetCollisionBox->setType(reactphysics3d::BodyType::STATIC);
}

void Planet::setVelocity(const float& speed)
{
	this->velocity = speed;
}

void Planet::setRotationSpeed(const DirectX::SimpleMath::Vector3& rotSpeed)
{
	this->rotSpeed = rotSpeed;
}

void Planet::setRotation(const DirectX::SimpleMath::Vector3& rotation)
{
	this->rotation = rotation;
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

DirectX::SimpleMath::Vector3 Planet::getAllGravFactor(const std::vector<Planet *>& planets, const DirectX::SimpleMath::Vector3& position) const
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

void Planet::rotateAroundPoint(const DirectX::XMFLOAT3& point)
{
	float deg = rotDegrees * (DirectX::XM_PI / 180.f);
	rotDegrees += this->velocity*0.1f;
	DirectX::SimpleMath::Vector3 factor(sin(deg), cos(deg), 1.0f);
	this->position = this->originPoint * factor;
}

void Planet::rotatePlanet()
{
	this->rotation += rotSpeed;
}
void Planet::setColor(const DirectX::SimpleMath::Vector3& color)
{
	//Set up color buffer
	this->colorBuffer.getData() = DirectX::XMFLOAT4(color.x, color.y, color.z, 0.1f);
	this->colorBuffer.applyData();
}

GravityField* Planet::getGravityField() const
{
	return this->gravField;
}

void Planet::drawPlanet()
{
	//this->mesh->position = this->position;
	//this->mesh->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
	//this->mesh->scale = scale;

	this->mesh->UpdateCB(this->position, DirectX::XMMatrixRotationRollPitchYawFromVector(rotation), scale);
	this->mesh->DrawWithMat();
}

void Planet::drawAtmosphere()
{
	if (atmosphere != nullptr)
	{
		this->atmosphere->UpdateCB(this->position, DirectX::XMMatrixIdentity(), this->scale + DirectX::XMFLOAT3(10, 10, 10));

		GPU::immediateContext->PSSetConstantBuffers(2, 1, colorBuffer.getReferenceOf());
		atmosphere->DrawWithMat();
	}
}
