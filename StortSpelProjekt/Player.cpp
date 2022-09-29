#include "PhysicsComponent.h"
#include "Player.h"
#include "DirectXMathHelper.h"
#include "Potion.h"
using namespace DirectX;

Player::Player(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
    :GameObject(useMesh, pos, rot, id), health(69), holdingItem(nullptr)
{
}

Player::Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:GameObject(objectPath, pos, rot, id), health(69), holdingItem(nullptr)
{
}

void Player::handleInputs()
{
    ////return true ONCE key press, trigger only ONCE
    //if (Input::KeyPress(KeyCode::W))
    //{
    //    std::cout << "W pressed \n";
    //}
    ////return true ONCE key release, trigger only ONCE
    //if (Input::KeyUp(KeyCode::A))
    //{
    //    std::cout << "A key released \n";
    //}

    //return true as long as key holding down, trigger every frame
    /*if (Input::KeyDown(KeyCode::W))
    {
        std::cout << "W key is holding down \n";
    }
    if (Input::KeyDown(KeyCode::S))
    {
        std::cout << "S key is holding down \n";
    }


    if (Input::KeyDown(KeyCode::A))
    {
        std::cout << "A key is holding down \n";
    }
    if (Input::KeyDown(KeyCode::D))
    {
        std::cout << "D key is holding down \n";
    }*/

}

void Player::move(const DirectX::XMFLOAT3& grav, const DirectX::XMVECTOR& cameraRight, const float& deltaTime)
{
    //Variables
    float rotationConstant = 0;
    
    playerUpVec = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 0.0f);
    playerForwardVec = DirectX::XMVector3Cross(cameraRight, playerUpVec);
    playerRightVec = DirectX::XMVector3Cross(playerUpVec, playerForwardVec);

    if (Input::KeyDown(KeyCode::W))
    {
         this->position += deltaTime * this->speedConstant * playerForwardVec;
    }

    else if (Input::KeyDown(KeyCode::S))
    {
        this->position -= deltaTime * this->speedConstant * playerForwardVec;
    }

    if (Input::KeyDown(KeyCode::D))
    {
        this->position += deltaTime * this->speedConstant * playerRightVec;
    }

    else if (Input::KeyDown(KeyCode::A))
    {
        this->position -= deltaTime * this->speedConstant * playerRightVec;
    }

    if (Input::KeyDown(KeyCode::E))
    {
        this->position.y += 0.1;
    }

    else if (Input::KeyDown(KeyCode::Q))
    {
        this->position.y -= 0.1;
    }
}

bool Player::pickupItem(Item* itemToPickup)
{
    bool successfulPickup = false;

    if (Input::KeyDown(KeyCode::SPACE))
    {
        if (this->withinRadius(itemToPickup, 5))
        {
            addItem(itemToPickup);

            Potion* tmp = dynamic_cast<Potion*>(itemToPickup);
            if (tmp)
                tmp->setPlayerptr(this);
               
            successfulPickup = true;
        }
    }
    
    return successfulPickup;
}

void Player::addItem(Item* itemToHold)
{
    if (!this->holdingItem)
        this->holdingItem = itemToHold;
}

void Player::addHealth(const int& healthToIncrease)
{
    this->health += healthToIncrease;
    //Prototyp för en cap så man inte kan få mer liv än en kapacitet
    if (this->health > 100)
    {
        this->health = 100;
    }
}

void Player::releaseItem()
{
    this->holdingItem = nullptr;
}

bool Player::withinRadius(Item* itemToLookWithinRadius, const float& radius) const
{
    DirectX::XMFLOAT3 objPos = itemToLookWithinRadius->getPos();
    DirectX::XMFLOAT3 selfPos = this->getPos();
    bool inRange = false;

    DirectX::XMFLOAT3 vecToObject = selfPos;
    subtractionXMFLOAT3(vecToObject, objPos);

    float lengthToVec = getLength(vecToObject);
    if (lengthToVec<=radius)
    {
        inRange = true;
    }

    return inRange;
}

void Player::update()
{
    if (holdingItem != nullptr)
    {
        holdingItem->setPos({ this->getPos().x + 1.0f, this->getPos().y + 0.5f, this->getPos().z + 0.5f });
        if (Input::KeyDown(KeyCode::R) && Input::KeyDown(KeyCode::R))
        {
            holdingItem->getPhysComp()->applyWorldTorque(reactphysics3d::Vector3(1000, 1000, 1000));
            holdingItem->getPhysComp()->applyForceToCenter(reactphysics3d::Vector3(10000, 10000, 10000));
            holdingItem->setPos({ this->getPos().x, this->getPos().y, this->getPos().z });
            holdingItem = nullptr;
        }
        else if (Input::KeyDown(KeyCode::T) && Input::KeyDown(KeyCode::T))
        {
            holdingItem->useItem();
            holdingItem = nullptr;
        }
    }
}
