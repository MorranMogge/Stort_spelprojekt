#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Player.h"
#include "DirectXMathHelper.h"
#include "Potion.h"
#include "BaseballBat.h"
#include "Component.h"
using namespace DirectX;

void Player::handleItems()
{
	DirectX::SimpleMath::Vector3 newPos = this->position;
	newPos += 4 * forwardVector;

	PhysicsComponent* itemPhysComp = holdingItem->getPhysComp();
	holdingItem->setPos(newPos);
	itemPhysComp->setPosition(reactphysics3d::Vector3({ newPos.x, newPos.y, newPos.z }));

	//Thorw the Item
	if (Input::KeyDown(KeyCode::R) && Input::KeyDown(KeyCode::R))
	{
		//Set dynamic so it can be affected by forces
		itemPhysComp->setType(reactphysics3d::BodyType::DYNAMIC);

		//Calculate the force vector
		DirectX::XMFLOAT3 temp;
		DirectX::XMStoreFloat3(&temp, (this->forwardVector * 5.f + this->upVector * 2.f));
		newNormalizeXMFLOAT3(temp);

		//Apply the force
		//itemPhysComp->applyLocalTorque(reactphysics3d::Vector3(temp.x * FORCE/2, temp.y * FORCE/2, temp.z * FORCE/2));
		itemPhysComp->applyForceToCenter(reactphysics3d::Vector3(temp.x * FORCE, temp.y * FORCE, temp.z * FORCE));

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

Player::Player(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
    :GameObject(useMesh, pos, rot, id), health(70), holdingItem(nullptr), speed(2.f)
{
	this->rotationMX = XMMatrixIdentity();
	dotValue = { 0.0f, 0.0f, 0.0f };
	dotProduct = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	normalVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

Player::Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:GameObject(objectPath, pos, rot, id), health(70), holdingItem(nullptr), speed(2.f)
{
	this->rotationMX = XMMatrixIdentity();
	dotValue = { 0.0f, 0.0f, 0.0f };
	dotProduct = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	normalVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
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

void Player::move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, float deltaTime, const bool& testingVec)
{
	if (!testingVec) normalVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 1.0f);
	else normalVector = DirectX::XMVectorSet(grav.x, grav.y, grav.z, 1.0f);

	upVector = XMVector3TransformCoord(DEFAULT_UP, rotation);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, rotation);

	upVector = DirectX::XMVector3Normalize(upVector);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	//Movement
	if (Input::KeyDown(KeyCode::SHIFT))
	{
		deltaTime *= 2.0f;
	}


	//X-Rotation
	dotProduct = DirectX::XMVector3Dot(normalVector, forwardVector);
	XMStoreFloat3(&dotValue, dotProduct);
	if (dotValue.x < -0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, deltaTime * -speed);
		rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, deltaTime * -speed);
	}
	else if (dotValue.x > 0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, deltaTime * speed);
		rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, deltaTime * speed);
	}

	//Z-Rotation
	dotProduct = DirectX::XMVector3Dot(normalVector, rightVector);
	XMStoreFloat3(&dotValue, dotProduct);
	if (dotValue.z < -0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, deltaTime * speed);
		rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, deltaTime * speed);
	}
	else if (dotValue.z > 0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, deltaTime * -speed);
		rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, deltaTime * -speed);
	}

	
	if (Input::KeyDown(KeyCode::W))
	{
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

bool Player::raycast(const std::vector<GameObject*>& gameObjects, DirectX::XMFLOAT3& hitPos, DirectX::XMFLOAT3& hitNormal)
{
	this->physComp->setPosition(reactphysics3d::Vector3({ this->position.x, this->position.y, this->position.z }));
	reactphysics3d::Ray ray(reactphysics3d::Vector3(this->position.x, this->position.y, this->position.z), reactphysics3d::Vector3(this->getRayCastPos()));
	reactphysics3d::RaycastInfo rayInfo;

	bool testingVec = false;
	int gameObjSize = gameObjects.size();
	for (int i = 1; i < gameObjSize; i++)
	{
		if (gameObjects[i]->getPhysComp()->raycast(ray, rayInfo))
		{
			//Maybe somehow return the index of the triangle hit to calculate new Normal
			hitPos = DirectX::XMFLOAT3(rayInfo.worldPoint.x, rayInfo.worldPoint.y, rayInfo.worldPoint.z);
			hitNormal = DirectX::XMFLOAT3(rayInfo.worldNormal.x, rayInfo.worldNormal.y, rayInfo.worldNormal.z);
			return true;
		}
	}
	return false;
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
	DirectX::SimpleMath::Quaternion dx11Quaternion = DirectX::XMQuaternionRotationMatrix(this->rotation);
	reactphysics3d::Quaternion reactQuaternion = reactphysics3d::Quaternion(dx11Quaternion.x, dx11Quaternion.y, dx11Quaternion.z, dx11Quaternion.w);
	this->physComp->setRotation(reactQuaternion);
	
	if (holdingItem != nullptr)
    {
		this->holdingItem->getPhysComp()->setRotation(reactQuaternion);
		this->handleItems();
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

reactphysics3d::Vector3 Player::getRayCastPos() const
{
	SimpleMath::Vector3 returnValue = this->position;
	returnValue += this->upVector * -2.f;
	return reactphysics3d::Vector3(returnValue.x, returnValue.y, returnValue.z);
}
