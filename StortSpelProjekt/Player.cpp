#include "Player.h"
#include "DirectXMathHelper.h"

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

void Player::move(const DirectX::XMFLOAT3& grav, const DirectX::XMVECTOR& cameraRight, const std::unique_ptr<DirectX::GamePad>& gamePad, const float& deltaTime)
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


    auto state = gamePad->GetState(0);

    if (state.IsConnected())
    {
        float posx = state.thumbSticks.leftX;
        float posy = state.thumbSticks.leftY;
        // These values are normalized to -1 to 1

        if (posy >= 1)
        {
            //std::cout << "Y >= 1\n";
            position += playerForwardVec;
        }
        else if (posy <= -1)
        {
            //std::cout << "Y >= -1\n";
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

        if (posy <= 0.90 && 0.50 <= posy && 0.1 <= posx && posx <= 0.90)
        {
            position += playerForwardVec;
            position += playerRightVec;
            //std::cout << "diagonal\n";
        }
        
        if (posy <= 0.90 && 0.50 <= posy && -1 <= posx && posx <= 0)
        {
            position += playerForwardVec;
            position -= playerRightVec;
        }
      
        //if (posy <= -1 && 0.f <= posy/* && 1.f <= posx && posx <= 0*/)
        //{
        //    position -= playerForwardVec;
        //    position -= playerRightVec;
        //}

        if (posy <= 0.0 && -0.50 <= posy && 0.1 <= posx && posx <= 0.90)
        {
            position += playerForwardVec;
            position += playerRightVec;
        }

        

        std::cout << "Posx: " << posx << "\t" << "Posy: " << posy << "\n";

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
    }
}

bool Player::pickupItem(Item* itemToPickup, const std::unique_ptr<DirectX::GamePad>& gamePad)
{
    bool successfulPickup = false;

    auto state = gamePad->GetState(0);

    if (Input::KeyDown(KeyCode::SPACE) || state.IsAPressed())
    {
        if (this->withinRadius(itemToPickup, 5))
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

void Player::addHealth(const int& healthToIncrease)
{
    this->health += healthToIncrease;
    //Prototyp f�r en cap s� man inte kan f� mer liv �n en kapacitet
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

void Player::update(const std::unique_ptr<DirectX::GamePad>& gamePad)
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
