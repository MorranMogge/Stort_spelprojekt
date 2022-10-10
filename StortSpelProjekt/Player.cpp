#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Player.h"
#include "DirectXMathHelper.h"
#include "Potion.h"
using namespace DirectX;

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

void Player::move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, float& deltaTime)
{
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
		deltaTime *= 3.0f;
	}

	if (Input::KeyDown(KeyCode::W))
	{
		position += forwardVector * deltaTime * 40.0f;
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
		else if (dotValue.x < -0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue.x > 0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
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
		position += forwardVector * deltaTime * 40.0f;
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
		else if (dotValue.x < -0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue.x > 0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
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
		position += forwardVector * deltaTime * 40.0f;
		dotProduct = DirectX::XMVector3Dot(cameraRight, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		if (dotValue.x < -0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue.z > 0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
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
		position += forwardVector * deltaTime * 40.0f;
		dotProduct = DirectX::XMVector3Dot(-cameraRight, rightVector);
		XMStoreFloat3(&dotValue, dotProduct);

		if (dotValue.x < -0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.02f);
		}
		else if (dotValue.x > 0.005f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
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
        holdingItem->setPos({ this->getPos().x + 1.0f, this->getPos().y + 0.5f, this->getPos().z + 0.5f });
        holdingItem->getPhysComp()->setPosition(reactphysics3d::Vector3({ this->getPos().x + 1.0f, this->getPos().y + 0.5f, this->getPos().z + 0.5f }));
        if (Input::KeyDown(KeyCode::R) && Input::KeyDown(KeyCode::R))
        {
            DirectX::XMFLOAT3 temp;
            DirectX::XMStoreFloat3(&temp, this->forwardVector);
            newNormalizeXMFLOAT3(temp);
            holdingItem->getPhysComp()->applyLocalTorque(reactphysics3d::Vector3(temp.x * 1000, temp.y * 1000, temp.z *1000));
            holdingItem->getPhysComp()->applyForceToCenter(reactphysics3d::Vector3(temp.x * 10000, temp.y * 10000, temp.z * 10000));
            holdingItem->setPos({ this->getPos().x, this->getPos().y, this->getPos().z });
            holdingItem = nullptr;
        }
        else if (Input::KeyDown(KeyCode::T) && Input::KeyDown(KeyCode::T))
        {
            holdingItem->useItem();
            repairCount++;
            std::cout << "Progress " << repairCount << "/4\n";
            //holdingItem->getPhysComp()->setPosition(reactphysics3d::Vector3({ 50.f, 50.f, 50.f }));
            holdingItem->getPhysComp()->setIsAllowedToSleep(true);
            holdingItem->getPhysComp()->setIsSleeping(true);
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