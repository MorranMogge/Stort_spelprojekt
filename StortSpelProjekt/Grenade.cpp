#include "stdafx.h"
#include "Grenade.h"
#include "DirectXMathHelper.h"
#include "Player.h"
#include "PhysicsComponent.h"

void Grenade::explode()
{
	int iterations = (int)gameObjects.size();
	for (int i = 0; i < iterations; i++)
	{
		if (gameObjects[i] == this) continue;
		if (this->withinRadious(gameObjects[i], 25))
		{
			DirectX::XMFLOAT3 explosionRange = getSubtractionXMFLOAT3(this->position, gameObjects[i]->getPos());
			float factor = 1.f/getLength(explosionRange);
			
			float newForce = this->physComp->getMass() * 10000;
			explosionRange = gameObjects[i]->getPosV3() - this->position;
			newNormalizeXMFLOAT3(explosionRange);
			scalarMultiplicationXMFLOAT3(newForce, explosionRange);

			//Add force to object
			Player* hitPlayer = dynamic_cast<Player*>(this->physComp->getParent());
			if (hitPlayer != nullptr) hitPlayer->hitByBat(reactphysics3d::Vector3(explosionRange.x, explosionRange.y, explosionRange.z));
			else physComp->applyForceToCenter(reactphysics3d::Vector3(explosionRange.x, explosionRange.y, explosionRange.z));
		}
	}
}

Grenade::Grenade(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:Item(objectPath, pos, rot, id), destructionIsImminent(false), timeToExplode(5.f)
{
}

Grenade::~Grenade()
{
}

void Grenade::updateExplosionCheck()
{
	if (destructionIsImminent && this->timer.getTimePassed(this->timeToExplode)) this->explode();
}

void Grenade::setGameObjects(const std::vector<GameObject*>& gameObjects)
{
	this->gameObjects = gameObjects;
}

void Grenade::useItem()
{
	this->destructionIsImminent = true;
}
