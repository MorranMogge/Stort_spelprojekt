#include "stdafx.h"
#include "BaseballBat.h"
#include "Player.h"
#include "PhysicsComponent.h"
#include "DirectXMathHelper.h"

using namespace DirectX;

BaseballBat::BaseballBat(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:Item(objectPath, pos, rot, id), player(nullptr), testObj(nullptr)
{
}

BaseballBat::~BaseballBat()
{
}

void BaseballBat::setPlayer(Player* player)
{
	this->player = player;
}

void BaseballBat::setTestObj(const std::vector<GameObject*>& objects)
{
	this->objects = objects;
}

void BaseballBat::useItem()
{
	SimpleMath::Vector3 batPos = this->player->getPos();
	batPos += this->player->getForwardVec() * 10;

	PhysicsComponent* playerComp = this->player->getPhysComp();
	playerComp->setPosition(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
	PhysicsComponent* physComp;
	for (int i = 2; i < objects.size(); i++)
	{
		physComp = objects[i]->getPhysComp();
		//if (physComp->getType() == reactphysics3d::BodyType::STATIC) continue;

		bool collided = playerComp->testBodiesOverlap(physComp);
		std::cout << "Result for id: " << i << ": " << collided << "\n";
		if (collided)
		{
			//Calculate the force vector
			float force = playerComp->getMass() * FORCECONSTANT;
			batPos = this->player->getForwardVec() * 10 + this->player->getUpVec();
			newNormalizeXMFLOAT3(batPos);
			scalarMultiplicationXMFLOAT3(force, batPos);

			//Add force to object
			physComp->applyForceToCenter(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
		}
	}
	

}

