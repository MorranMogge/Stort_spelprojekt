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
    if (Input::KeyDown(KeyCode::W))
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
    }

}

void Player::move(float deltaTime)
{
    //Variables
    float rotationConstant = 0;

    
    //hämta down vektor
    //XMFLOAT3 upVector = -downVector;



    //Forward
    if (Input::KeyDown(KeyCode::W))                                 //W
    {
        this->setRot(XMFLOAT3(0, 0 + rotationConstant, 0));

       
        if (Input::KeyDown(KeyCode::A))                             //WA
        {
            
        }
        else if (Input::KeyDown(KeyCode::D))                        //WD
        {
            
        }

    }


    //Back
    if (Input::KeyDown(KeyCode::S))                                 //S
    {
        if (Input::KeyDown(KeyCode::A))                             //SA
        {

        }
        else if (Input::KeyDown(KeyCode::D))                        //SD
        {

        }
    }


    //Right
    if (Input::KeyDown(KeyCode::D))                                 //D
    {
        if (Input::KeyDown(KeyCode::W))                             //DW
        {

        }
        else if (Input::KeyDown(KeyCode::S))                        //DS
        {

        }
    }    


    //Left
    if (Input::KeyDown(KeyCode::A))                                 //A
    {
        if (Input::KeyDown(KeyCode::W))                             //AW
        {

        }
        else if (Input::KeyDown(KeyCode::S))                        //AS
        {

        }
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
