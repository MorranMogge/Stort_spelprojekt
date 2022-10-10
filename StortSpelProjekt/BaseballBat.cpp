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

void BaseballBat::setTestObj(GameObject* testObj)
{
	this->testObj = testObj;
}

void BaseballBat::useItem()
{
	SimpleMath::Vector3 batPos = this->player->getPos();
	batPos += this->player->getForwardVec() * 10;
	this->player->getPhysComp()->setPosition(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
	
	bool collided = this->player->getPhysComp()->testBodiesOverlap(this->testObj->getPhysComp());
	std::cout << "result: " << collided << "\n";
	if (collided)
	{
		float force = this->player->getPhysComp()->getMass() * FORCECONSTANT;
		batPos = this->player->getForwardVec() * 10 + this->player->getUpVec();
		newNormalizeXMFLOAT3(batPos);
		scalarMultiplicationXMFLOAT3(force, batPos);
		this->testObj->getPhysComp()->applyForceToCenter(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
	}

}

