#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Player.h"
#include "DirectXMathHelper.h"
#include "Potion.h"
#include "BaseballBat.h"
#include "Component.h"
using namespace DirectX;

Player::Player(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field)
    :GameObject(useMesh, pos, rot, id, field), health(70), holdingItem(nullptr)
{
	this->rotationMX = XMMatrixIdentity();
	dotValue = { 0.0f, 0.0f, 0.0f };
	dotProduct = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	normalVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(1, 3), 1);

	//Item Icon
	float constant =5.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> playernames{ "player1.png", "player2.png", "player3.png", "player4.png" };
	this->playerIcon = new BilboardObject(playernames, iconPos);
	this->playerIcon->setOffset(constant);
}

Player::Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field)
	:GameObject(objectPath, pos, rot, id, field), health(70), holdingItem(nullptr)
{
	this->rotationMX = XMMatrixIdentity();
	dotValue = { 0.0f, 0.0f, 0.0f };
	dotProduct = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	normalVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(1, 3), 1);

	//Item Icon
	float constant = 5.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> playernames{ "player1.png", "player2.png", "player3.png", "player4.png" };
	this->playerIcon = new BilboardObject(playernames, iconPos);
	this->playerIcon->setOffset(constant);
}

Player::~Player()
{
	if (this->playerIcon != nullptr)
	{
		delete playerIcon;
	}
	if (this->particles != nullptr)
	{
		delete particles;
	}
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

void Player::move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, float& deltaTime)
{
    //Variables
	normalVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 1.0f);
	upVector = XMVector3TransformCoord(DEFAULT_UP, rotation);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, rotation);

	upVector = DirectX::XMVector3Normalize(upVector);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	//X-Rotation
	dotProduct = DirectX::XMVector3Dot(normalVector, forwardVector);
	XMStoreFloat3(&dotValue, dotProduct);
	if (dotValue.x < -0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, -0.01f);
		rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, -0.01f);
	}
	else if (dotValue.x > 0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, 0.01f);
		rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, 0.01f);
	}

	//Z-Rotation
	dotProduct = DirectX::XMVector3Dot(normalVector, rightVector);
	XMStoreFloat3(&dotValue, dotProduct);
	if (dotValue.z < -0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, 0.01f);
		rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, 0.01f);
	}
	else if (dotValue.z > 0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, -0.01f);
		rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, -0.01f);
	}

	//Movement
	if (Input::KeyDown(KeyCode::SHIFT))
	{
		deltaTime *= 2.0f;
	}


	if (Input::KeyDown(KeyCode::W))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * 25.0f;
		dotProduct = DirectX::XMVector3Dot(cameraForward, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		//Walking cross
		if (Input::KeyDown(KeyCode::D))
		{
			if (dotValue.x < -0.45f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
			}
			else if (dotValue.x > -0.55f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
		else if (Input::KeyDown(KeyCode::A))
		{
			if (dotValue.x < 0.45f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
			}
			else if (dotValue.x > 0.55f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}

		//Walking normally
		else if (dotValue.x < -0.05f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.07f);
		}
		else if (dotValue.x > 0.05f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.07f);
		}
		else
		{
			//Checking where it is
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
	}
	else if (Input::KeyDown(KeyCode::S))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * 25.0f;
		dotProduct = DirectX::XMVector3Dot(-cameraForward, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		//Walking cross
		if (Input::KeyDown(KeyCode::D))
		{
			if (dotValue.x < 0.45f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
			}
			else if (dotValue.x > 0.55f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
		else if (Input::KeyDown(KeyCode::A))
		{
			if (dotValue.x < -0.45f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
			}
			else if (dotValue.x > -0.55f)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}

		//Walking normally
		else if (dotValue.x < -0.05f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.07f);
		}
		else if (dotValue.x > 0.05f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.07f);
		}
		else
		{
			//Checking where it is
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(-cameraForward, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
	}
	else if (Input::KeyDown(KeyCode::D))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * 25.0f;
		dotProduct = DirectX::XMVector3Dot(cameraRight, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		if (dotValue.x < -0.05f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.07f);
		}
		else if (dotValue.z > 0.05f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.07f);
		}
		else
		{
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(cameraRight, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
	}
	else if (Input::KeyDown(KeyCode::A))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * 25.0f;
		dotProduct = DirectX::XMVector3Dot(-cameraRight, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		if (dotValue.x < -0.05f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.07f);
		}
		else if (dotValue.x > 0.05f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.07f);
		}
		else
		{
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(-cameraRight, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2)
			{
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
	}


	if (!Input::KeyDown(KeyCode::W) && !Input::KeyDown(KeyCode::A) && !Input::KeyDown(KeyCode::S) && !Input::KeyDown(KeyCode::D))
	{
		this->moveKeyPressed = false;
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
			{
				tmp->setPlayerptr(this);
				successfulPickup = true;
				tmp->setPickedUp(true);
			}

			
			holdingItem->getPhysComp()->getRigidBody()->resetForce();
			holdingItem->getPhysComp()->getRigidBody()->resetTorque();
            holdingItem->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);

        }
    }
    
    return successfulPickup;
}

void Player::addItem(Item* itemToHold)
{
    if (!this->holdingItem)
	{
		this->holdingItem = itemToHold;
		this->holdingItem->setPickedUp(true);
	}
    holdingItem->getPhysComp()->setType(reactphysics3d::BodyType::DYNAMIC);
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
	if (this->holdingItem != nullptr)
	{
		this->holdingItem->setPickedUp(false);
		this->holdingItem = nullptr;
	}
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

bool Player::repairedShip() const
{
    return repairCount>=4;
}

void Player::update()
{
    if (holdingItem != nullptr)
    {
        DirectX::SimpleMath::Vector3 newPos = this->position; 
        newPos += 4*forwardVector;
        
        PhysicsComponent* itemPhysComp = holdingItem->getPhysComp();
        holdingItem->setPos(newPos);
        itemPhysComp->setPosition(reactphysics3d::Vector3({ newPos.x, newPos.y, newPos.z}));
        
        //Thorw the Item
        if (Input::KeyDown(KeyCode::R) && Input::KeyDown(KeyCode::R))
        {
            //Set dynamic so it can be affected by forces
	        itemPhysComp->setType(reactphysics3d::BodyType::DYNAMIC);

            //Calculate the force vector
            DirectX::XMFLOAT3 temp;
            DirectX::XMStoreFloat3(&temp, (this->forwardVector*5+this->upVector));
            newNormalizeXMFLOAT3(temp);

            //Apply the force
            itemPhysComp->applyLocalTorque(reactphysics3d::Vector3(temp.x * 500, temp.y * 500, temp.z * 500));
            itemPhysComp->applyForceToCenter(reactphysics3d::Vector3(temp.x * 1000, temp.y * 1000, temp.z * 1000));
			holdingItem->setPickedUp(false);

            //You no longer "own" the item
            holdingItem = nullptr;
        }
        //Use the Item
        else if (Input::KeyDown(KeyCode::T) && Input::KeyDown(KeyCode::T))
        {
            itemPhysComp->setType(reactphysics3d::BodyType::DYNAMIC);
            holdingItem->useItem();
            itemPhysComp->setIsAllowedToSleep(true);
            itemPhysComp->setIsSleeping(true);
			holdingItem->setPickedUp(false);
            holdingItem = nullptr;
        }
    }
	//Update icon movement
	if (this->playerIcon != nullptr)
	{
		float constant = playerIcon->getOffset();
		DirectX::XMFLOAT3 upDir = this->getUpDirection();
		DirectX::XMFLOAT3 itemPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
		this->playerIcon->setPosition(this->position + itemPos);
	}
	//Update particle movement
	if (this->particles != nullptr && moveKeyPressed)
	{
		//tStruct.resetStartTime();
		DirectX::XMFLOAT3 rot = this->getRotOrientedToGrav();
		this->particles->setPosition(this->position);
		this->particles->setRotation(this->getUpDirection());
		this->particles->updateBuffer();
	}
}

void Player::drawIcon(int playerIndex)
{
	if (this->playerIcon != nullptr)
	{
		this->playerIcon->bindAndDraw(playerIndex, 0);
	}
}

void Player::drawParticles()
{
	if (this->particles != nullptr && moveKeyPressed)
	{
		this->particles->BindAndDraw(0);
	}
}

DirectX::XMVECTOR Player::getUpVec() const
{
	return this->normalVector;
}

DirectX::XMVECTOR Player::getForwardVec() const
{
	return this->forwardVector;
}

DirectX::XMVECTOR Player::getRightVec() const
{
	return this->rightVector;
}

DirectX::XMMATRIX Player::getRotationMX() const
{
	return this->rotationMX;
}