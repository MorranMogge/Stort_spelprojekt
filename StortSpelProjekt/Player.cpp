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

void Player::move(DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& rotation, const DirectX::XMFLOAT3& grav, const DirectX::XMVECTOR& cameraRight, float deltaTime)
{
    //Variables
    rotation.y = std::clamp(rotation.y, 0.0f, XM_PI * 2);

    rotationMX = XMMatrixRotationRollPitchYawFromVector(rotation);
    forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMX);
    rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, rotationMX);
    upVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 0.0f);

    //Rotating in the X-axis
    dotProduct = XMVector3Dot(upVector, forwardVector);
    XMStoreFloat3(&dotValue, dotProduct);

    if (dotValue.x < -0.1f)
    {
        rotation.x -= 0.05f;
    }
    else if (dotValue.x > 0.1f)
    {
        rotation.x += 0.05f;
    }

    //Rotating in the Z-axis
    dotProduct = XMVector3Dot(upVector, rightVector);
    XMStoreFloat3(&dotValue, dotProduct);

    if (dotValue.z < -0.1f)
    {
        rotation.z += 0.05f;
    }
    else if (dotValue.x > 0.1f)
    {
        rotation.z -= 0.05f;
    }

    /*OutputDebugString(L"PLAYER DOT PRODUCT: ");
    OutputDebugString(L"\n");
    OutputDebugString(std::to_wstring(newPos.x).c_str());
    OutputDebugString(L"\n");
    OutputDebugString(L"\n");*/

    //The actual movement
    if (Input::KeyDown(KeyCode::W))
    {
        //position += playerForwardVec * 0.3f;
        position += forwardVector * deltaTime * 40;
        
        if (Input::KeyDown(KeyCode::D))
        {
            rotation.y = XM_PIDIV4;
        }
        else if (Input::KeyDown(KeyCode::A))
        {
            rotation.y = XM_PI + XM_PIDIV2 + XM_PIDIV4;
        }
        else
        {
            if ((rotation.y - XM_PI) < 0.0f)
            {
                rotation.y -= 0.06f;
            }

            if ((rotation.y - XM_PI) > 0.0f)
            {
                rotation.y += 0.06f;
            }
        }
    }

    else if (Input::KeyDown(KeyCode::S))
    {
        //position += playerForwardVec * 0.3f;
        position += forwardVector * deltaTime * 40;

        if (Input::KeyDown(KeyCode::D))
        {
            rotation.y = XM_PIDIV2 + XM_PIDIV4;
        }
        else if (Input::KeyDown(KeyCode::A))
        {
            rotation.y = XM_PI + XM_PIDIV4;
        }
        else
        {
            if (rotation.y < XM_PI)
            {
                rotation.y += 0.06f;
            }

            if (rotation.y > XM_PI)
            {
                rotation.y -= 0.06f;
            }
        }
    }

    else if (Input::KeyDown(KeyCode::D))
    {
        //position += playerForwardVec * 0.3f;
        position += forwardVector * deltaTime * 40;

        if (rotation.y >= XM_PI * 2)
        {
            rotation.y = 0.0f;
        }

        if (rotation.y > (XM_PI + XM_PIDIV2) || rotation.y < XM_PIDIV2)
        {
            rotation.y += 0.06f;
        }

        else if (rotation.y < (XM_PI + XM_PIDIV2))
        {
            rotation.y -= 0.06f;
        }
    }

    else if (Input::KeyDown(KeyCode::A))
    {
        //position += playerForwardVec * 0.3f;
        position += forwardVector * deltaTime * 40;

        if (rotation.y <= 0.0f)
        {
            rotation.y = XM_PI * 2;
        }

        if (rotation.y < XM_PIDIV2 || rotation.y > (XM_PI + XM_PIDIV2))
        {
            rotation.y -= 0.06f;
        }

        else if (rotation.y > XM_PIDIV2)
        {
            rotation.y += 0.06f;
        }
    }

    //Help keys
    if (Input::KeyDown(KeyCode::E))
    {
        //position.y += 1.5f;
        position.y +=  0.1f;
    }

    else if (Input::KeyDown(KeyCode::Q))
    {
        //position.y -= 1.5f;
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
