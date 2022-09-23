#include "Player.h"

Player::Player(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
    :GameObject(useMesh, pos, rot, id), health(69), holdingItem(nullptr)
{
}

Player::Player(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
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

void Player::move(DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& rotation, const DirectX::XMFLOAT3& grav, const DirectX::XMVECTOR& cameraRight, float deltaTime)
{
    //Variables
    float rotationConstant = 0;
    
    playerUpVec = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 0.0f);
    playerForwardVec = DirectX::XMVector3Cross(cameraRight, playerUpVec);
    playerRightVec = DirectX::XMVector3Cross(playerUpVec, playerForwardVec);

    if (Input::KeyDown(KeyCode::W))
    {
        position += playerForwardVec;
    }

    else if (Input::KeyDown(KeyCode::S))
    {
        position -= playerForwardVec;
    }

    if (Input::KeyDown(KeyCode::D))
    {
        position += playerRightVec;
    }

    else if (Input::KeyDown(KeyCode::A))
    {
        position -= playerRightVec;
    }

    if (Input::KeyDown(KeyCode::E))
    {
        position.y += 0.1;
    }

    else if (Input::KeyDown(KeyCode::Q))
    {
        position.y -= 0.1;
    }
}

bool Player::pickupItem(Item* itemToPickup)
{

    bool successfulPickup = false;

    if (Input::KeyDown(KeyCode::SPACE))
    {
        if (this->withinRadious(itemToPickup, 50))
        {
            addItem(itemToPickup);
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

void Player::releaseItem()
{
    this->holdingItem = nullptr;
}

bool Player::withinRadius(Item* itemToLookWithinRadius, float radius) const
{
    using namespace DirectX;

    XMFLOAT3 objPos = itemToLookWithinRadius->getPos();
    XMFLOAT3 selfPos = this->getPos();
    bool inRange = false;

    //X range
    if (objPos.x <= selfPos.x + radius && objPos.x >= selfPos.x - radius)
    {
        //Y range
        if (objPos.y <= selfPos.y + radius && objPos.y >= selfPos.y - radius)
        {
            //Z range
            if (objPos.z <= selfPos.z + radius && objPos.z >= selfPos.z - radius)
            {
                inRange = true;
            }
        }
    }

    return inRange;
}
