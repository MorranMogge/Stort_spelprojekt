#include "stdafx.h"
#include "BaseballBat.h"
#include "Player.h"
#include "PhysicsComponent.h"
#include "DirectXMathHelper.h"

using namespace DirectX;

BaseballBat::BaseballBat(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:Item(objectPath, pos, rot, id), player(nullptr), force(0.f)
{
	force = FORCECONSTANT;
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
	batPos = this->player->getPos();
	batPos += this->player->getForwardVec() * 10;
	savedPos = this->getPosV3(); //Used to reset the baseball bats position at the end of the function

	PhysicsComponent* batComp = this->getPhysComp();
	PhysicsComponent* physComp;

	batComp->setPosition(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
	batComp->setScale(DirectX::XMFLOAT3(4.0f, 4.0f, 4.0f));

	bool collided = false;
	for (int i = 4; i < objects.size(); i++)
	{
		if (this == objects[i]) continue;
		physComp = objects[i]->getPhysComp();

		collided = batComp->testBodiesOverlap(physComp);
		
		if (collided)
		{
			Player* otherPlayer = dynamic_cast<Player*>(physComp->getParent()); //If we add a function "isPlayer()" in GameObject we do not have to type cast
			if (otherPlayer != nullptr) std::cout << "You hit another Player!!!\n";

			physComp->setType(reactphysics3d::BodyType::DYNAMIC);
			//Calculate the force vector
			force = batComp->getMass() * force;
			batPos = objects[i]->getPosV3() - this->player->getPosV3();
			batPos += this->player->getUpVec();
			newNormalizeXMFLOAT3(batPos);
			scalarMultiplicationXMFLOAT3(force, batPos);

			//Add force to object
			physComp->applyForceToCenter(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
		}
	}

	batComp->setScale(DirectX::XMFLOAT3(4 * 0.35f, 4 * 0.35f, 4 * 0.35f));
	this->setPos(savedPos);
	batComp->setPosition(reactphysics3d::Vector3(savedPos.x, savedPos.y, savedPos.z));

}

