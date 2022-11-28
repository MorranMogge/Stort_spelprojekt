#include "BaseballBat.h"
#include "ObjectId.h"

using namespace DirectX;



BaseballBat::BaseballBat(const int& onlineId)
	:Item(onlineId, ObjID::BAT), force(0.f)
{
	force = FORCECONSTANT;
}

BaseballBat::~BaseballBat()
{
}

//void BaseballBat::setPlayer(Player* player)
//{
//	this->player = player;
//}

//void BaseballBat::setGameObjects(const std::vector<Player*>& objects)
//{
//	for (int i = 0; i < objects.size(); i++)
//	{
//		//if (objects[i] != this->player) this->objects.push_back(objects[i]);
//		this->objects.push_back(objects[i]);
//	}
//}

void BaseballBat::use(const Player* playerHoldingItem)
{
	std::cout << "use(const Player* playerHoldingItem) on server isnt in use!\n";
	//DirectX::SimpleMath::Vector3 batPos;
	//batPos = DirectX::SimpleMath::Vector3(playerHoldingItem->getposition('x'), playerHoldingItem->getposition('y'), playerHoldingItem->getposition('z'));
	//batPos += playerHoldingItem->getForwardVector() * 10;
	//savedPos = this->getPosV3(); //Used to reset the baseball bats position at the end of the function

	//PhysicsComponent* batComp = this->getPhysComp();
	//PhysicsComponent* physComp;
	//batComp->setPosition(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
	//batComp->setScale(DirectX::XMFLOAT3(2 * 4.0f, 2 * 4.0f, 2 * 4.0f));
	//bool collided = false;
	//for (int i = 0; i < objects.size(); i++)
	//{
	//	if (this == objects[i] || objects[i] == playerHoldingItem) continue;

	//	physComp = objects[i]->getPhysComp();
	//	if (physComp->getType() == reactphysics3d::BodyType::STATIC) continue;

	//	collided = batComp->testBodiesOverlap(physComp);

	//	if (collided)
	//	{
	//		Player* otherPlayer = dynamic_cast<Player*>(physComp->getParent()); //If we add a function "isPlayer()" in GameObject we do not have to type cast

	//		physComp->setType(reactphysics3d::BodyType::DYNAMIC);
	//		//Calculate the force vector
	//		float newForce = batComp->getMass() * force;
	//		batPos = objects[i]->getPosV3() - playerHoldingItem->getPosV3();
	//		batPos += playerHoldingItem->getUpVector();
	//		newNormalizeXMFLOAT3(batPos);
	//		scalarMultiplicationXMFLOAT3(newForce, batPos);

	//		//Add force to object
	//		if (otherPlayer != nullptr) this->sendForceToServer(batPos, otherPlayer->getOnlineID()); //otherPlayer->hitByBat(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
	//		else physComp->applyForceToCenter(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
	//	}
	//}

	//batComp->setScale(DirectX::XMFLOAT3(4 * 0.35f, 4 * 0.35f, 4 * 0.35f));
	//this->setPos(savedPos);
	//batComp->setPosition(reactphysics3d::Vector3(savedPos.x, savedPos.y, savedPos.z));
}