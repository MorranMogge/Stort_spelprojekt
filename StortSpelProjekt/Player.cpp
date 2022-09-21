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
    float rotationConstant = 0;
    
    playerUpVec = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 0.0f);
    playerForwardVec = DirectX::XMVector3Cross(cameraRight, playerUpVec);
    playerRightVec = DirectX::XMVector3Cross(playerUpVec, playerForwardVec);

    forwardVec = XMVector3AngleBetweenVectors(zVec, playerForwardVec);
    XMFLOAT3 newPos;
    XMStoreFloat3(&newPos, forwardVec);

    dotVector = XMVector3Dot(tempRightVec, playerForwardVec);
    XMFLOAT3 newPos2;
    XMStoreFloat3(&newPos2, dotVector);

    OutputDebugString(L"PLAYER DOT: ");
    OutputDebugString(L"\n");
    OutputDebugString(std::to_wstring(newPos2.y).c_str());
    OutputDebugString(L"\n");
    OutputDebugString(L"\n");

    XMFLOAT3 newPos3;
    XMStoreFloat3(&newPos3, playerForwardVec);

    /*OutputDebugString(L"PLAYER FORWARD: ");
    OutputDebugString(std::to_wstring(newPos3.x).c_str());
    OutputDebugString(L"\n");
    OutputDebugString(std::to_wstring(newPos3.y).c_str());
    OutputDebugString(L"\n");
    OutputDebugString(std::to_wstring(newPos3.z).c_str());
    OutputDebugString(L"\n");
    OutputDebugString(L"\n");*/

    /*if (Input::KeyPress(KeyCode::W))
    {
        if (newPos2.x < 0.0f)
        {
            rotation.y -= newPos.y;
        }
        else
        {
            rotation.y += newPos.y;
        }

        zVec = playerForwardVec;
        tempRightVec = playerRightVec;
    }

    if (Input::KeyDown(KeyCode::O))
    {
        position += tempRightVec;
    }*/

    if (Input::KeyDown(KeyCode::W))
    {
        position += playerForwardVec;
        /*rotation.x += newPos3.z * 0.02;
        rotation.z -= newPos3.x * 0.02;*/
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
