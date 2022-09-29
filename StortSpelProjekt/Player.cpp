#include "Player.h"
#include <algorithm>

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

void Player::move(DirectX::XMVECTOR cameraRight, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& rotation, const DirectX::XMFLOAT3& grav, float deltaTime)
{
    normalVector = XMVectorSet(-grav.x, -grav.y, -grav.z, 1.0f);

    rotationMX = XMMatrixRotationRollPitchYawFromVector(rotation);
    upVector = XMVector3TransformCoord(DEFAULT_UP, rotationMX);
    rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, rotationMX);
    forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMX);
    
    //X-Rotation
    dotProduct = XMVector3Dot(normalVector, forwardVector);
    XMStoreFloat3(&dotValue, dotProduct);
    if (dotValue.x < -0.1f)
    {
        rotation.x -= 0.05f;
    }
    else if (dotValue.x > 0.1f)
    {
        rotation.x += 0.05f;
    }

    //Z-Rotation
    dotProduct = XMVector3Dot(normalVector, rightVector);
    XMStoreFloat3(&dotValue, dotProduct);
    if (dotValue.z < -0.1f)
    {
        rotation.z += 0.05f;
    }
    else if (dotValue.z > 0.1f)
    {
        rotation.z -= 0.05f;
    }

    if (Input::KeyDown(KeyCode::W))
    {
        position += forwardVector * 0.3f;
        //rotation.y = 0.0f;
    }

    if (Input::KeyDown(KeyCode::S))
    {
        position -= forwardVector * 0.3f;
        //rotation.y = XM_PI;
    }

    if (Input::KeyDown(KeyCode::D))
    {
        position += rightVector * 0.3f;
        //rotation.y = XM_PIDIV2;
    }

    if (Input::KeyDown(KeyCode::A))
    {
        position -= rightVector * 0.3f;
        //rotation.y = piDiv2;
    }

    //Help keys
    if (Input::KeyDown(KeyCode::E))
    {
        position.y += 0.1f;
    }

    else if (Input::KeyDown(KeyCode::Q))
    {
        position.y -= 0.1f;
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

DirectX::XMVECTOR Player::getUpVector()
{
    return this->upVector;
}

DirectX::XMVECTOR Player::getForwardVector()
{
    return this->forwardVector;
}

DirectX::XMVECTOR Player::getRightVector()
{
    return this->rightVector;
}