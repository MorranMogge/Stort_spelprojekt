#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Player.h"
#include "DirectXMathHelper.h"
#include "Potion.h"
#include "BaseballBat.h"
#include "Component.h"
using namespace DirectX;

Player::Player(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:GameObject(useMesh, pos, rot, id), health(70), holdingItem(nullptr)
{
	this->rotationMX = XMMatrixIdentity();
	dotValue = { 0.0f, 0.0f, 0.0f };
	dotProduct = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	normalVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

Player::Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:GameObject(objectPath, pos, rot, id), health(70), holdingItem(nullptr)
{
	this->rotationMX = XMMatrixIdentity();
	dotValue = { 0.0f, 0.0f, 0.0f };
	dotProduct = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	normalVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

void Player::handleInputs()
{
}

void Player::move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, float& deltaTime)
{
    //Variables
	normalVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 1.0f);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	upVector = XMVector3TransformCoord(DEFAULT_UP, rotation);

	upVector = DirectX::XMVector3Normalize(upVector);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	//X-Rotation
	dotProduct = DirectX::XMVector3Dot(normalVector, forwardVector);
	XMStoreFloat3(&dotValue, dotProduct);
	if (dotValue.x < -0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, -0.009f);
		rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, -0.009f);
	}
	else if (dotValue.x > 0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, 0.009f);
		rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, 0.009f);
	}

	//Z-Rotation
	dotProduct = DirectX::XMVector3Dot(normalVector, rightVector);
	XMStoreFloat3(&dotValue, dotProduct);
	if (dotValue.z < -0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, 0.009f);
		rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, 0.009f);
	}
	else if (dotValue.z > 0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, -0.009f);
		rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, -0.009f);
	}

	//PC movement
	if (Input::KeyDown(KeyCode::SHIFT))
	{
		deltaTime *= 1.5f;
	}

	if (Input::KeyDown(KeyCode::W))
	{
		keyboardMove = true;
		position += forwardVector * deltaTime * 25.0f;
		dotProduct = DirectX::XMVector3Dot(cameraForward, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		//Walking cross
		if (Input::KeyDown(KeyCode::D))
		{
			if (dotValue.x > -0.4f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
			else if (dotValue.x < -0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
		}
		else if (Input::KeyDown(KeyCode::A))
		{
			if (dotValue.x < 0.4f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
			else if (dotValue.x > 0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
		}

		//Walking normally
		else if (dotValue.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
		else if (dotValue.x > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
		else
		{
			//Checking where it is
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
		}
	}

	else if (Input::KeyDown(KeyCode::S))
	{
		keyboardMove = true;
		position += forwardVector * deltaTime * 25.0f;
		dotProduct = DirectX::XMVector3Dot(-cameraForward, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		//Walking cross
		if (Input::KeyDown(KeyCode::D))
		{
			if (dotValue.x < 0.4f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
			else if (dotValue.x > 0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
		}
		else if (Input::KeyDown(KeyCode::A))
		{
			if (dotValue.x > -0.4f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
			else if (dotValue.x < -0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
		}

		//Walking normally
		else if (dotValue.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
		else if (dotValue.x > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
		else
		{
			//Checking where it is
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(-cameraForward, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
		}
	}

	else if (Input::KeyDown(KeyCode::D))
	{
		keyboardMove = true;
		position += forwardVector * deltaTime * 25.0f;
		dotProduct = DirectX::XMVector3Dot(cameraRight, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		if (dotValue.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
		else if (dotValue.z > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
		else
		{
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(cameraRight, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
		}
	}

	else if (Input::KeyDown(KeyCode::A))
	{
		keyboardMove = true;
		position += forwardVector * deltaTime * 25.0f;
		dotProduct = DirectX::XMVector3Dot(-cameraRight, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		if (dotValue.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
		else if (dotValue.x > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
		else
		{
			dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(-cameraRight, forwardVector);
			XMStoreFloat3(&dotValue, dotProduct);
			if (dotValue.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
		}
	}
}

void Player::moveController(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, const std::unique_ptr<DirectX::GamePad>& gamePad, float& deltaTime)
{
	if (!keyboardMove)
	{
		//Controller movement
		auto state = gamePad->GetState(0);

		if (state.IsConnected())
		{
			posX = state.thumbSticks.leftX;
			posY = state.thumbSticks.leftY;

			normalVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 1.0f);

			forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
			rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
			upVector = XMVector3TransformCoord(DEFAULT_UP, rotation);

			upVector = DirectX::XMVector3Normalize(upVector);
			rightVector = DirectX::XMVector3Normalize(rightVector);
			forwardVector = DirectX::XMVector3Normalize(forwardVector);

			if (state.IsAPressed())
			{
				deltaTime *= 1.2f;
			}

			//Walk forward
			if (posY > 0.0f)
			{
				dotProduct = DirectX::XMVector3Dot(cameraForward, rightVector);
				XMStoreFloat3(&dotValue, dotProduct);

				//Walking cross
				if (posX > 0.0f)
				{
					totalPos = posX + posY;
					position += forwardVector * totalPos * deltaTime * 20.0f;

					if (dotValue.x > -0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
					else if (dotValue.x < -0.8f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
				}
				else if (posX < 0.0f)
				{
					totalPos = abs(posX) + posY;
					position += forwardVector * totalPos * deltaTime * 20.0f;

					if (dotValue.x < 0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
					else if (dotValue.x > 0.8f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
				}
				//Walking normally
				else
				{
					position += forwardVector * posY * deltaTime * 25.0f;
					if (dotValue.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
					else if (dotValue.x > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
					else
					{
						//Checking where it is
						dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
						XMStoreFloat3(&dotValue, dotProduct);
						if (dotValue.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
					}
				}
			}

			//Walk backward
			else if (posY < 0.0f)
			{
				dotProduct = DirectX::XMVector3Dot(-cameraForward, rightVector);
				XMStoreFloat3(&dotValue, dotProduct);

				//Walking cross
				if (posX > 0.0f)
				{
					totalPos = posX + abs(posY);
					position += forwardVector * totalPos * deltaTime * 20.0f;

					if (dotValue.x < 0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
					else if (dotValue.x > 0.8f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
				}
				else if (posX < 0.0f)
				{
					totalPos = abs(posX) + abs(posY);
					position += forwardVector * totalPos * deltaTime * 20.0f;

					if (dotValue.x > -0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
					else if (dotValue.x < -0.8f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
				}
				//Walking normally
				else
				{
					position += forwardVector * posY * deltaTime * -25.0f;

					if (dotValue.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
					else if (dotValue.x > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
					else
					{
						//Checking where it is
						dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(-cameraForward, forwardVector);
						XMStoreFloat3(&dotValue, dotProduct);
						if (dotValue.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
					}
				}
			}

			//Walk right
			else if (posX > 0.0f)
			{
				position += forwardVector * posX * deltaTime * 25.0f;
				dotProduct = DirectX::XMVector3Dot(cameraRight, rightVector);
				XMStoreFloat3(&dotValue, dotProduct);

				if (dotValue.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
				else if (dotValue.z > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
				else
				{
					dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(cameraRight, forwardVector);
					XMStoreFloat3(&dotValue, dotProduct);
					if (dotValue.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
				}
			}

			//Walk left
			else if (posX < 0.0f)
			{
				position += forwardVector * posX * deltaTime * -25.0f;
				dotProduct = DirectX::XMVector3Dot(-cameraRight, rightVector);
				XMStoreFloat3(&dotValue, dotProduct);

				if (dotValue.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
				else if (dotValue.z > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
				else
				{
					dotProduct = DirectX::XMVector3AngleBetweenNormalsEst(-cameraRight, forwardVector);
					XMStoreFloat3(&dotValue, dotProduct);
					if (dotValue.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
				}
			}
		}
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
        this->holdingItem = itemToHold;
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
	if (lengthToVec <= radius)
	{
		inRange = true;
	}

	return inRange;
}

bool Player::repairedShip() const
{
	return repairCount >= 4;
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
            holdingItem = nullptr;
        }
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