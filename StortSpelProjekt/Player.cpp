#include "Player.h"

Player::Player(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
    :GameObject(useMesh, pos, rot, id)
{
}

Player::Player(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:GameObject(objectPath, pos, rot, id)
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
    playerUpVec = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 0.0f);
    
    rotationMX = XMMatrixRotationRollPitchYawFromVector(rotation);
    playerRightVec = XMVector3TransformCoord(DEFAULT_RIGHT, rotationMX);
    //playerForwardVec = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMX);

    playerForwardVec = DirectX::XMVector3Cross(playerRightVec, playerUpVec);
    //playerRightVec = DirectX::XMVector3Cross(playerUpVec, playerForwardVec

    if (Input::KeyDown(KeyCode::W))
    {
        position += playerForwardVec * 0.5;
        rotation.x += 0.023;
    }

    else if (Input::KeyDown(KeyCode::S))
    {
        position -= playerForwardVec * 0.5;
        rotation.x -= 0.022;
    }

    if (Input::KeyDown(KeyCode::D))
    {
        position += playerRightVec * 0.5;
        rotation.z -= 0.022;
    }

    else if (Input::KeyDown(KeyCode::A))
    {
        position -= playerRightVec * 0.5;
        rotation.z += 0.022;
    }

    //Help keys
    if (Input::KeyDown(KeyCode::E))
    {
        position.y += 1.0f;
    }

    else if (Input::KeyDown(KeyCode::Q))
    {
        position.y -= 1.0f;
    }
}

bool Player::getPickup(GameObject* pickup)
{
    bool pickedUp = false;


    if (Input::KeyDown(KeyCode::SPACE))                           //SPACE
    {
        //Check if should pick up
        if (this->withinRadious(pickup, 50) && this->pickup == nullptr)
        {
            this->pickup = pickup;
        }  
    }

    return pickedUp;
}

void Player::releasePickup()
{
    this->pickup = nullptr;
}
