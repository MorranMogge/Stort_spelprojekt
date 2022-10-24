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
}

void Player::move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, float deltaTime, const bool& testingVec)
{
	if (dedge) return;
	if (!testingVec) normalVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 1.0f);
	else normalVector = DirectX::XMVectorSet(grav.x, grav.y, grav.z, 1.0f);

	upVector = XMVector3TransformCoord(DEFAULT_UP, rotation);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	upVector = XMVector3TransformCoord(DEFAULT_UP, rotation);

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

void Player::hitByBat(const reactphysics3d::Vector3& force)
{
	this->dedge = true;
	this->physComp->applyForceToCenter(force);
	this->physComp->applyWorldTorque(force);

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

bool Player::checkForStaticCollision(const std::vector<GameObject*>& gameObjects)
{
	SimpleMath::Vector3 vecPoint = this->position;
	vecPoint += 1.f * forwardVector;
	reactphysics3d::Vector3 point(vecPoint.x, vecPoint.y, vecPoint.z);

	int gameObjSize = gameObjects.size();
	for (int i = 1; i < gameObjSize; i++)
	{
		if (gameObjects[i]->getPhysComp()->testPointInside(point)) 
		{
			if (gameObjects[i]->getPhysComp()->getType() == reactphysics3d::BodyType::STATIC) this->position -= 1.f * forwardVector;
			return true;
		}
	}
	return false;
}

bool Player::raycast(const std::vector<GameObject*>& gameObjects, DirectX::XMFLOAT3& hitPos, DirectX::XMFLOAT3& hitNormal)
{
	if (!dedge)
	{
		DirectX::SimpleMath::Quaternion dx11Quaternion = DirectX::XMQuaternionRotationMatrix(this->rotation);
		reactphysics3d::Quaternion reactQuaternion = reactphysics3d::Quaternion(dx11Quaternion.x, dx11Quaternion.y, dx11Quaternion.z, dx11Quaternion.w);
		this->physComp->setRotation(reactQuaternion);
		if (holdingItem != nullptr) this->holdingItem->getPhysComp()->setRotation(reactQuaternion);
	}
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

bool Player::getHitByBat() const
{
	return dedge;
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

void Player::update()
{
	this->handleInputs();
	if (holdingItem != nullptr)
	{
		this->handleItems();
	}
	if (!dedge) this->physComp->setPosition(reactphysics3d::Vector3({ this->position.x, this->position.y, this->position.z }));
	else if (dedge)
	{
		this->position = this->physComp->getPosV3();
		reactphysics3d::Quaternion reactQuaternion;
		DirectX::SimpleMath::Quaternion dx11Quaternion;
		reactQuaternion = this->physComp->getRotation();
		dx11Quaternion = DirectX::SimpleMath::Quaternion(DirectX::SimpleMath::Vector4(reactQuaternion.x, reactQuaternion.y, reactQuaternion.z, reactQuaternion.w));
		this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(dx11Quaternion.ToEuler());
		if (timer.getTimePassed(7.f)) 
		{ 
			dedge = false; 
			this->physComp->resetForce();
			this->physComp->resetTorque();
			this->physComp->setType(reactphysics3d::BodyType::STATIC); 
			this->position = SimpleMath::Vector3(22, 10, 20);
			this->physComp->setPosition(reactphysics3d::Vector3({ this->position.x, this->position.y, this->position.z }));
			this->physComp->setType(reactphysics3d::BodyType::KINEMATIC);
		}
	}
}