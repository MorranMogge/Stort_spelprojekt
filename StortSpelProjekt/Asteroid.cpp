#include "stdafx.h"
#include "Asteroid.h"
#include "DirectXMathHelper.h"

void Asteroid::explode(std::vector<Planet*>& planets, std::vector<GameObject*>& objects)
{
	for (int j = 0; j < objects.size(); j++)
	{
		DirectX::XMFLOAT3 vecToObject = this->position;
		subtractionXMFLOAT3(vecToObject, objects[j]->getPos());
		if (getLength(vecToObject) >= 25.f)
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
}

Asteroid::Asteroid(Mesh* mesh, PhysicsWorld& physWorld, DirectX::XMFLOAT3 spawnPos, DirectX::XMFLOAT3 travelDirection, float speed)
{
	this->asteroidMesh = mesh;
	this->position = spawnPos;
	this->direction = travelDirection;
	this->speed = speed;
	this->scale = DirectX::XMFLOAT3(1, 1, 1);
	this->physComp = physWorld.returnAddedPhysComponent();
	this->physComp->setType(reactphysics3d::BodyType::KINEMATIC);
}

Asteroid::~Asteroid()
{
}

void Asteroid::update(std::vector<Planet*>& planets, std::vector<GameObject*>& objects)
{
	additionXMFLOAT3(this->position, getScalarMultiplicationXMFLOAT3(speed, direction));
	this->physComp->setPosition(reactphysics3d::Vector3(this->position.x, this->position.y, this->position.z));
	for (int i = 0; i < planets.size(); i++)
	{
		if (this->physComp->testBodiesOverlap(planets[i]->getPlanetCollider())) { this->explode(planets, objects); return; }
	}
}

void Asteroid::draw()
{
	this->asteroidMesh->position = this->position;
	this->asteroidMesh->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f));
	this->asteroidMesh->scale = this->scale;

	this->asteroidMesh->UpdateCB();
	this->asteroidMesh->DrawWithMat();
}
