#include "Player.h"
#include "DirectXMathHelper.h"

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

void Player::move(DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& rotation, const DirectX::XMFLOAT3& grav, const DirectX::XMVECTOR& cameraRight, std::unique_ptr<DirectX::GamePad>& gamePad, float deltaTime)
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


    auto state = gamePad->GetState(0);

    if (state.IsConnected())
    {
        if (state.IsAPressed())
        {
            std::cout << "A is pressed\n";
            gamePad->SetVibration(0, 1.f, 1.f);
        }
        else
        {
            gamePad->SetVibration(0, 0.f, 0.f);

        }

        // Do action for button A being down

        if (state.buttons.y)
        {
            std::cout << "Y is pressed\n";
        }
        // Do action for button Y being down

        if (state.IsDPadLeftPressed())
        {
            std::cout << "DPAD Left is down\n";
        }
        // Do action for DPAD Left being down

        if (state.dpad.up || state.dpad.down || state.dpad.left || state.dpad.right)
        {
            std::cout << "DPAD\n";
        }
        // Do action based on any DPAD change

        float posx = state.thumbSticks.leftX;
        float posy = state.thumbSticks.leftY;
        // These values are normalized to -1 to 1

        if (posy >= 1)
        {
            std::cout << "Y >= 1\n";
            position += playerForwardVec;
        }
        else if (posy <= -1)
        {
            std::cout << "Y >= -1\n";
            position -= playerForwardVec;
        }

        if (posx >= 1)
        {
            position += playerRightVec;

        }
        else if (posx <= -1)
        {
            position -= playerRightVec;
        }
        
        float throttle = state.triggers.right;
        // This value is normalized 0 -> 1

        if (state.IsLeftTriggerPressed())
        {
            std::cout << "Left trigger pressed\n";
        }
        // Do action based on a left trigger pressed more than halfway

        if (state.IsViewPressed())
        {
            std::cout << "backbutton is pressed\n";
        }

        if (state.IsLeftStickPressed())
        {
            std::cout << "Left stick down\n";
        }



        // This is an alias for the Xbox 360 'Back' button
        // which is called 'View' on the Xbox One controller.
    }


}

bool Player::pickupItem(Item* itemToPickup, std::unique_ptr<DirectX::GamePad>& gamePad)
{
    bool successfulPickup = false;

    auto state = gamePad->GetState(0);

    
    if (Input::KeyDown(KeyCode::SPACE) || state.IsAPressed())
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

bool Player::withinRadius(Item* itemToLookWithinRadius, float radius) const
{
    using namespace DirectX;

    XMFLOAT3 objPos = itemToLookWithinRadius->getPos();
    XMFLOAT3 selfPos = this->getPos();
    bool inRange = false;

    XMFLOAT3 vecToObject = selfPos;
    subtractionXMFLOAT3(vecToObject, objPos);

    float lengthToVec = getLength(vecToObject);
    if (lengthToVec<=radius)
    {
        inRange = true;
    }

    return inRange;
}

void Player::update(std::unique_ptr<DirectX::GamePad>& gamePad)
{
    if (holdingItem != nullptr)
    {
        auto state = gamePad->GetState(0);

        holdingItem->setPos({ this->getPos().x + 1.0f, this->getPos().y + 0.5f, this->getPos().z + 0.5f });
        if (Input::KeyDown(KeyCode::R) && Input::KeyDown(KeyCode::R) || state.IsBPressed() && state.IsBPressed())
        {
            holdingItem->setPos({ this->getPos().x, this->getPos().y, this->getPos().z });
            holdingItem = nullptr;
        }
        else if (Input::KeyDown(KeyCode::T) && Input::KeyDown(KeyCode::T) || state.IsYPressed() && state.IsYPressed())
        {
            holdingItem->useItem();
            holdingItem = nullptr;
        }
    }
}
