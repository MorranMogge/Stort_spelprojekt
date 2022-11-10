#include "stdafx.h"
#include "Asteroid.h"
#include "DirectXMathHelper.h"

void Asteroid::explode(std::vector<Planet*>& planets, std::vector<GameObject*>& objects)
{
	for (int j = 0; j < objects.size(); j++)
	{
		DirectX::XMFLOAT3 vecToObject = this->position;
		subtractionXMFLOAT3(vecToObject, objects[j]->getPos());
		if (getLength(vecToObject) <= 25.f)
		{
			objects[j]->getPhysComp()->setType(reactphysics3d::BodyType::DYNAMIC);
			DirectX::XMFLOAT3 explosionRange = getSubtractionXMFLOAT3(this->position, objects[j]->getPos());

			explosionRange = objects[j]->getPosV3() - this->position;
			float factor = 1.f / getLength(explosionRange);
			float newForce;
			//newNormalizeXMFLOAT3(explosionRange);
			Player* hitPlayer = dynamic_cast<Player*>(objects[j]);
			if (hitPlayer != nullptr)
			{
				newForce = this->physComp->getMass() * 2500 * factor;
				scalarMultiplicationXMFLOAT3(newForce, explosionRange);
				hitPlayer->hitByBat(reactphysics3d::Vector3(explosionRange.x, explosionRange.y, explosionRange.z));
			}
			//Add force to object
			else
			{
				newForce = this->physComp->getMass() * 5000 * factor;
				scalarMultiplicationXMFLOAT3(newForce, explosionRange);
				objects[j]->getPhysComp()->applyForceToCenter(reactphysics3d::Vector3(explosionRange.x, explosionRange.y, explosionRange.z));
			}
		}
	}
	this->inactive = true;
}

Asteroid::Asteroid(Mesh* mesh, PhysicsWorld& physWorld)
{
	this->asteroidMesh = mesh;
	this->scale = DirectX::XMFLOAT3(1, 1, 1);
	//this->physComp = physWorld.returnAddedPhysComponent(reactphysics3d::CollisionShapeName::SPHERE, DirectX::XMFLOAT3(0,0,0), this->scale);
	//this->physComp->setType(reactphysics3d::BodyType::KINEMATIC);
	this->inactive = false;
}

Asteroid::~Asteroid()
{
}

bool Asteroid::isActive() const
{
	return !inactive;
}

void Asteroid::spawnAsteroid(DirectX::XMFLOAT3 spawnPos, DirectX::XMFLOAT3 direction, float speed)
{
	this->position = spawnPos;
	this->direction = direction;
	this->speed = speed;
	this->inactive = false;
	//this->physComp->setPosition(reactphysics3d::Vector3(this->position.x, this->position.y, this->position.z));
}

void Asteroid::moveAsteroid(const float& dt, std::vector<Planet*>& planets, std::vector<GameObject*>& objects)
{
	if (this->inactive) return;
	additionXMFLOAT3(this->position, getScalarMultiplicationXMFLOAT3(speed*dt, direction));
	this->physComp->setPosition(reactphysics3d::Vector3(this->position.x, this->position.y, this->position.z));
	for (int i = 0; i < planets.size(); i++)
	{
		DirectX::XMFLOAT3 vecToPlanet = this->position;
		subtractionXMFLOAT3(vecToPlanet, planets[i]->getPlanetPosition());
		if ((getLength(vecToPlanet) - (this->scale.x + planets[i]->getSize())) <= 0)
		{ 
			this->explode(planets, objects); 
			return; 
		}
	}
}

void Asteroid::draw()
{
	if (this->inactive) return;
	this->asteroidMesh->position = this->position;
	this->asteroidMesh->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f));
	this->asteroidMesh->scale = this->scale;

	this->asteroidMesh->UpdateCB(this->position, DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f)), this->scale);
	this->asteroidMesh->DrawWithMat();
}
