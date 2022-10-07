#include "BaseballBat.h"
#include "Player.h"
#include "PhysicsComponent.h"
#include "DirectXMathHelper.h"

using namespace DirectX;

void BaseballBat::setPlayer(Player* player)
{
	this->player = player;
}

void BaseballBat::setTestObj(GameObject* testObj)
{
	this->testObj = testObj;
}

void BaseballBat::useItem()
{
	SimpleMath::Vector3 batPos = this->player->getPos();
	batPos += this->player->getForwardVec() * 20;
	if (this->player->getPhysComp()->getCollider()->testPointInside(this->testObj->getPhysComp()->getPosition()))
	{
		float force = this->player->getPhysComp()->getMass() * FORCECONSTANT;
		batPos += this->player->getUpVec();
		newNormalizeXMFLOAT3(batPos);
		scalarMultiplicationXMFLOAT3(force, batPos);
		this->testObj->getPhysComp()->applyForceToCenter(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
	}

}
