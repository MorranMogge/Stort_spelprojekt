#include "stdafx.h"
#include "BaseballBat.h"
#include "Player.h"
#include "PhysicsComponent.h"
#include "DirectXMathHelper.h"
#include "Client.h"
#include "PacketsDataTypes.h"
#include "PacketEnum.h"

using namespace DirectX;

void BaseballBat::sendForceToServer(const DirectX::SimpleMath::Vector3& hitForce, const int& playerID)
{
	PlayerHit hitInfo;
	hitInfo.packetId = PacketType::PLAYERHIT;
	hitInfo.playerId = playerID;
	hitInfo.xForce = batPos.x;
	hitInfo.yForce = batPos.y;
	hitInfo.zForce = batPos.z;
	std::cout << "Hit player!!!!\n";
	this->client->sendStuff<PlayerHit>(hitInfo);
}

BaseballBat::BaseballBat(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field)
	:Item(objectPath, pos, rot, id, onlineId, 2, field), player(nullptr), force(0.f)
{
	force = FORCECONSTANT;

	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(2, 5), 2);

	//Item Icon
	float constant = 2.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
    std::vector<std::string> tempStr{ "icon_sword.png", "icon_sword2.png" };
	this->itemIcon = new BilboardObject(tempStr, iconPos);
	this->itemIcon->setOffset(constant);
}

BaseballBat::~BaseballBat()
{
}

void BaseballBat::setPlayer(Player* player)
{
	this->player = player;
}

void BaseballBat::setClient(Client* client)
{
	this->client = client;
}

void BaseballBat::setTestObj(const std::vector<GameObject*>& objects)
{
	this->objects = objects;
}

void BaseballBat::setGameObjects(const std::vector<Player*>& objects)
{
	for (int i = 0; i < objects.size(); i++)
	{
		//if (objects[i] != this->player) this->objects.push_back(objects[i]);
		this->objects.push_back(objects[i]);
	}
}

void BaseballBat::useItem()
{
	std::cout << "Used bat!\n";
	batPos = this->player->getPos();
	batPos += this->player->getForwardVector() * 10;
	savedPos = this->getPosV3(); //Used to reset the baseball bats position at the end of the function

    PhysicsComponent* batComp = this->getPhysComp();
    PhysicsComponent* physComp;

    batComp->setPosition(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
    batComp->setScale(DirectX::XMFLOAT3(4.0f, 4.0f, 4.0f));

	bool collided = false;
	for (int i = 0; i < objects.size(); i++)
	{
		if (this == objects[i] || objects[i] == this->player) continue;

        physComp = objects[i]->getPhysComp();
        if (physComp->getType() == reactphysics3d::BodyType::STATIC) continue;

        collided = batComp->testBodiesOverlap(physComp);

        if (collided)
        {
            Player* otherPlayer = dynamic_cast<Player*>(physComp->getParent()); //If we add a function "isPlayer()" in GameObject we do not have to type cast

			physComp->setType(reactphysics3d::BodyType::DYNAMIC);
            //Calculate the force vector
            float newForce = batComp->getMass() * force;
            batPos = objects[i]->getPosV3() - this->player->getPosV3();
            batPos += this->player->getUpVector();
            newNormalizeXMFLOAT3(batPos);
            scalarMultiplicationXMFLOAT3(newForce, batPos);

			//Add force to object
			if (otherPlayer != nullptr) this->sendForceToServer(batPos, otherPlayer->getOnlineID()); //otherPlayer->hitByBat(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
			else physComp->applyForceToCenter(reactphysics3d::Vector3(batPos.x, batPos.y, batPos.z));
		}
	}

	batComp->setScale(DirectX::XMFLOAT3(4 * 0.35f, 4 * 0.35f, 4 * 0.35f));
    this->setPos(savedPos);
    batComp->setPosition(reactphysics3d::Vector3(savedPos.x, savedPos.y, savedPos.z));

}
