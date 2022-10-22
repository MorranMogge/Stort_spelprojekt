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
	resultVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

Player::Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:GameObject(objectPath, pos, rot, id), health(70), holdingItem(nullptr)
{
	this->rotationMX = XMMatrixIdentity();
	resultVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

void Player::handleInputs()
{
}

void Player::move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, float& deltaTime)
{
	if (Input::KeyDown(KeyCode::SHIFT))
	{
		deltaTime *= 1.5f;
	}

	speed = 0.001f / deltaTime;

	//Calculations
	normalVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 1.0f);
	rightVector = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	forwardVector = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	normalVector = DirectX::XMVector3Normalize(normalVector);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	//X-Rotation
	resultVector = DirectX::XMVector3Dot(normalVector, forwardVector);
	if (resultVector.x < 0.0f)
	{
		resultVector = DirectX::XMVector3Cross(rightVector, normalVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(forwardVector, resultVector);

		rotation *= DirectX::XMMatrixRotationAxis(rightVector, -resultVector.x);
		rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, -resultVector.x);
	}
	else if (resultVector.x > 0.0f)
	{
		resultVector = DirectX::XMVector3Cross(rightVector, normalVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(forwardVector, resultVector);

		rotation *= DirectX::XMMatrixRotationAxis(rightVector, resultVector.x);
		rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, resultVector.x);
	}

	//Z-Rotation
	resultVector = DirectX::XMVector3Dot(normalVector, rightVector);
	if (resultVector.z < 0.0f)
	{
		resultVector = DirectX::XMVector3Cross(normalVector, forwardVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(rightVector, resultVector);

		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, resultVector.z);
		rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, resultVector.z);
	}
	else if (resultVector.z > 0.0f)
	{
		resultVector = DirectX::XMVector3Cross(normalVector, forwardVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(rightVector, resultVector);

		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, -resultVector.z);
		rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, -resultVector.z);
	}

	//Jumping - Need to change
	if (Input::KeyDown(KeyCode::SPACE))
	{
		position += normalVector * deltaTime * 100.0f;
	}

	//PC movement
	if (Input::KeyDown(KeyCode::W))
	{
		controllerConnected = false;
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(cameraForward, rightVector);

		//Walking cross
		if (Input::KeyDown(KeyCode::D))
		{
			if (resultVector.x > -0.4f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * 10.f);
			else if (resultVector.x < -0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * -10.f);
		}
		else if (Input::KeyDown(KeyCode::A))
		{
			if (resultVector.x < 0.4f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * -10.f);
			else if (resultVector.x > 0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * 10.f);
		}

		//Walking normally
		else if (resultVector.x < -0.1f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -speed);

			/*resultVector = DirectX::XMVector3AngleBetweenNormals(cameraForward, forwardVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.5f);*/
		}
		else if (resultVector.x > 0.1f)
		{
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, speed);

			/*resultVector = DirectX::XMVector3AngleBetweenNormals(cameraForward, forwardVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.5f);*/
		}
		else
		{
			//Checking where it is
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
			//if (resultVector.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * 5.f);
		}
	}

	else if (Input::KeyDown(KeyCode::S))
	{
		controllerConnected = false;
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(-cameraForward, rightVector);

		//Walking cross
		if (Input::KeyDown(KeyCode::D))
		{
			if (resultVector.x < 0.4f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * -10.f);
			else if (resultVector.x > 0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * 10.f);
		}
		else if (Input::KeyDown(KeyCode::A))
		{
			if (resultVector.x > -0.4f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * 10.f);
			else if (resultVector.x < -0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * -10.f);
		}

		//Walking normally
		else if (resultVector.x < -0.1f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -speed);
		else if (resultVector.x > 0.1f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, speed);
		else
		{
			//Checking where it is
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraForward, forwardVector);
			//if (resultVector.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * 5.f);
		}
	}

	else if (Input::KeyDown(KeyCode::D))
	{
		controllerConnected = false;
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(cameraRight, rightVector);

		if (resultVector.x < -0.1f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -speed);
		else if (resultVector.z > 0.1f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, speed);
		else
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraRight, forwardVector);
			//if (resultVector.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * 5.f);
		}
	}

	else if (Input::KeyDown(KeyCode::A))
	{
		controllerConnected = false;
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(-cameraRight, rightVector);

		if (resultVector.x < -0.1f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -speed);
		else if (resultVector.x > 0.1f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, speed);
		else
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraRight, forwardVector);
			//if (resultVector.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, deltaTime * 5.f);
		}
	}
}

void Player::moveController(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, const std::unique_ptr<DirectX::GamePad>& gamePad, float& deltaTime)
{
	if (controllerConnected)
	{
		//Controller movement
		auto state = gamePad->GetState(0);
		controllerConnected = state.IsConnected();

		posX = state.thumbSticks.leftX;
		posY = state.thumbSticks.leftY;

		if (state.IsAPressed())
		{
			deltaTime *= 1.2f;
		}

		//Walk forward
		if (posY > 0.0f)
		{
			resultVector = DirectX::XMVector3Dot(cameraForward, rightVector);

			//Walking cross
			if (posX > 0.0f)
			{
				totalPos = posX + posY;
				position += forwardVector * totalPos * deltaTime * 20.0f;

				if (resultVector.x > -0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
				else if (resultVector.x < -0.8f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
			}
			else if (posX < 0.0f)
			{
				totalPos = abs(posX) + posY;
				position += forwardVector * totalPos * deltaTime * 20.0f;

				if (resultVector.x < 0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
				else if (resultVector.x > 0.8f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
			}
			//Walking normally
			else
			{
				position += forwardVector * posY * deltaTime * 25.0f;
				if (resultVector.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
				else if (resultVector.x > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
				else
				{
					//Checking where it is
					resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
					if (resultVector.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
				}
			}
		}

		//Walk backward
		else if (posY < 0.0f)
		{
			resultVector = DirectX::XMVector3Dot(-cameraForward, rightVector);

			//Walking cross
			if (posX > 0.0f)
			{
				totalPos = posX + abs(posY);
				position += forwardVector * totalPos * deltaTime * 20.0f;

				if (resultVector.x < 0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
				else if (resultVector.x > 0.8f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
			}
			else if (posX < 0.0f)
			{
				totalPos = abs(posX) + abs(posY);
				position += forwardVector * totalPos * deltaTime * 20.0f;

				if (resultVector.x > -0.6f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
				else if (resultVector.x < -0.8f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
			}
			//Walking normally
			else
			{
				position += forwardVector * posY * deltaTime * -25.0f;

				if (resultVector.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
				else if (resultVector.x > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
				else
				{
					//Checking where it is
					resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraForward, forwardVector);
					if (resultVector.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
				}
			}
		}

		//Walk right
		else if (posX > 0.0f)
		{
			position += forwardVector * posX * deltaTime * 25.0f;
			resultVector = DirectX::XMVector3Dot(cameraRight, rightVector);

			if (resultVector.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
			else if (resultVector.z > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
			else
			{
				resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraRight, forwardVector);
				if (resultVector.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}

		//Walk left
		else if (posX < 0.0f)
		{
			position += forwardVector * posX * deltaTime * -25.0f;
			resultVector = DirectX::XMVector3Dot(-cameraRight, rightVector);

			if (resultVector.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
			else if (resultVector.z > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
			else
			{
				resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraRight, forwardVector);
				if (resultVector.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}
	}
}

bool Player::pickupItem(Item* itemToPickup)
{
	bool successfulPickup = false;

	if (Input::KeyDown(KeyCode::ENTER))
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
			DirectX::XMStoreFloat3(&temp, (this->forwardVector * 5 + this->normalVector));
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