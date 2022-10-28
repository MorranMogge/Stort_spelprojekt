#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Player.h"
#include "DirectXMathHelper.h"
#include "Potion.h"
#include "BaseballBat.h"
#include "Component.h"
#include "Mesh.h"
using namespace DirectX;

void Player::resetRotationMatrix()
{
	this->rotationMX = DirectX::XMMatrixIdentity();
	this->rotation = DirectX::XMMatrixIdentity();
}

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
		DirectX::XMStoreFloat3(&temp, (this->forwardVector * 5.f + this->normalVector * 2.f));
		newNormalizeXMFLOAT3(temp);

		//Apply the force
		//itemPhysComp->applyLocalTorque(reactphysics3d::Vector3(temp.x * FORCE/2, temp.y * FORCE/2, temp.z * FORCE/2));
		itemPhysComp->applyForceToCenter(reactphysics3d::Vector3(temp.x * FORCE, temp.y * FORCE, temp.z * FORCE));

		//You no longer "own" the item
		holdingItem->setPickedUp(false);
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

Player::Player(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& team , GravityField* field)
    :GameObject(useMesh, pos, rot, id, field), health(70), holdingItem(nullptr), team(team)
{
		this->rotationMX = XMMatrixIdentity();
		resultVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	
		normalVector = DirectX::XMVectorSet(this->getUpDirection().x, this->getUpDirection().y, this->getUpDirection().z, 1.0f);
		rightVector = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
		forwardVector = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
		normalVector = DirectX::XMVector3Normalize(normalVector);
		rightVector = DirectX::XMVector3Normalize(rightVector);
		forwardVector = DirectX::XMVector3Normalize(forwardVector);
		this->rotate();

	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(1, 3), 1);

	//Item Icon
	float constant =7.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> playernames{ "player1.png", "player2.png", "player3.png", "player4.png" };
	this->playerIcon = new BilboardObject(playernames, iconPos);
	this->playerIcon->setOffset(constant);

	//Team switch
	switch (team)
	{
	case 0:
		this; mesh->matKey[0] = "spaceshipTexture1.jpg"; break;
	case 1:
		this; break;
	}
}

Player::Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& team, GravityField* field)
	:GameObject(objectPath, pos, rot, id, field), health(70), holdingItem(nullptr), team(team)
{
	this->rotationMX = XMMatrixIdentity();
	resultVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	

	normalVector = DirectX::XMVectorSet(this->getUpDirection().x, this->getUpDirection().y, this->getUpDirection().z, 1.0f);
	rightVector = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	forwardVector = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	normalVector = DirectX::XMVector3Normalize(normalVector);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);
	this->rotate();

	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(1, 3), 1);

	//Item Icon
	float constant = 7.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> playernames{ "player1.png", "player2.png", "player3.png", "player4.png" };
	this->playerIcon = new BilboardObject(playernames, iconPos);
	this->playerIcon->setOffset(constant);

	//Team switch
	switch (team)
	{
	case 0:
		mesh->matKey[0] = "spaceshipTexture1.jpg"; break;
	case 1:
		break;
	}
}

void Player::handleInputs()
{
}

bool Player::movingCross(const DirectX::XMVECTOR& cameraForward, float deltaTime)
{
	//Calculations
	rightVector = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	forwardVector = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	northEastVector = DirectX::XMVector3TransformCoord(NORTH_EAST, rotationMX);
	northWestVector = DirectX::XMVector3TransformCoord(NORTH_WEST, rotationMX);
	southEastVector = DirectX::XMVector3TransformCoord(SOUTH_EAST, rotationMX);
	southWestVector = DirectX::XMVector3TransformCoord(SOUTH_WEST, rotationMX);

	northEastVector = DirectX::XMVector3Normalize(northEastVector);
	northWestVector = DirectX::XMVector3Normalize(northWestVector);
	southEastVector = DirectX::XMVector3Normalize(southEastVector);
	southWestVector = DirectX::XMVector3Normalize(southWestVector);

	//Walk North-East
	if (Input::KeyDown(KeyCode::W) && Input::KeyDown(KeyCode::D))
	{
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(cameraForward, rightVector);

		if (resultVector.x > -0.4f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, northEastVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.3f);
		}
		else if (resultVector.x < -0.6f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, northEastVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.3f);
		}

		this->rotate();
		return true;
	}

	//Walk North-West
	else if (Input::KeyDown(KeyCode::W) && Input::KeyDown(KeyCode::A))
	{
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(cameraForward, rightVector);

		if (resultVector.x < 0.4f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, northWestVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.3f);
		}
		else if (resultVector.x > 0.6f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, northWestVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.3f);
		}

		this->rotate();
		return true;
	}

	//Walk South-East
	else if (Input::KeyDown(KeyCode::S) && Input::KeyDown(KeyCode::D))
	{
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(-cameraForward, rightVector);

		if (resultVector.x < 0.4f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, southEastVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.3f);
		}
		else if (resultVector.x > 0.6f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, southEastVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.3f);
		}

		this->rotate();
		return true;
	}

	//Walk South-West
	else if (Input::KeyDown(KeyCode::S) && Input::KeyDown(KeyCode::A))
	{
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(-cameraForward, rightVector);

		if (resultVector.x > -0.4f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, southWestVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.3f);
		}
		else if (resultVector.x < -0.6f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, southWestVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.3f);
		}

		this->rotate();
		return true;
	}

	return false;
}

void Player::rotate()
{
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
}

void Player::move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, float deltaTime, const bool& testingVec)
{
	if (dedge) return;
	if (!testingVec) normalVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 1.0f);
	else normalVector = DirectX::XMVectorSet(grav.x, grav.y, grav.z, 1.0f);

	//Calculations
	rightVector = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	forwardVector = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	normalVector = DirectX::XMVector3Normalize(normalVector);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	//Jumping
	if (jumpHeight >= jumpAllowed && Input::KeyPress(KeyCode::SPACE))
	{
		jumpHeight = 0.f;
	}
	else if (jumpHeight < jumpAllowed)
	{
		position += normalVector * jumpHeight * deltaTime;
		jumpHeight += 3000.f * deltaTime;
	}

	//Running
	if (Input::KeyDown(KeyCode::SHIFT))
	{
		deltaTime *= 1.5f;
	}


	//PC movement
	if (movingCross(cameraForward, deltaTime)) {}

	//Walking forward
	else if (Input::KeyDown(KeyCode::W))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(cameraForward, rightVector);

		if (resultVector.x < -0.05f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.5f);
		}
		else if (resultVector.x > 0.05f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.5f);
		}
		else
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
			if (resultVector.x > XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.5f);
		}
		this->rotate();
	}

	//Walking backward
	else if (Input::KeyDown(KeyCode::S))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(-cameraForward, rightVector);

		if (resultVector.x < -0.05f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraForward, forwardVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.5f);
		}
		else if (resultVector.x > 0.05f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraForward, forwardVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.5f);
		}
		else
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraForward, forwardVector);
			if (resultVector.x > XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.5f);
		}
		rotate();
	}

	//Walking right
	else if (Input::KeyDown(KeyCode::D))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(cameraRight, rightVector);

		if (resultVector.x < -0.05f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraRight, forwardVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.5f);
		}
		else if (resultVector.z > 0.05f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraRight, forwardVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.5f);
		}
		else
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraRight, forwardVector);
			if (resultVector.x > XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.5f);
		}
		rotate();
	}

	//Walking left
	else if (Input::KeyDown(KeyCode::A))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * 25.0f;
		resultVector = DirectX::XMVector3Dot(-cameraRight, rightVector);

		if (resultVector.x < -0.05f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraRight, forwardVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.5f);
		}
		else if (resultVector.x > 0.05f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraRight, forwardVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.5f);
		}
		else
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraRight, forwardVector);
			if (resultVector.x > XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.5f);
		}
		rotate();
	}
}

bool Player::moveCrossController(const DirectX::XMVECTOR& cameraForward, float deltaTime)
{
	//Calculations
	rightVector = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	forwardVector = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	northEastVector = DirectX::XMVector3TransformCoord(NORTH_EAST, rotationMX);
	northWestVector = DirectX::XMVector3TransformCoord(NORTH_WEST, rotationMX);
	southEastVector = DirectX::XMVector3TransformCoord(SOUTH_EAST, rotationMX);
	southWestVector = DirectX::XMVector3TransformCoord(SOUTH_WEST, rotationMX);

	northEastVector = DirectX::XMVector3Normalize(northEastVector);
	northWestVector = DirectX::XMVector3Normalize(northWestVector);
	southEastVector = DirectX::XMVector3Normalize(southEastVector);
	southWestVector = DirectX::XMVector3Normalize(southWestVector);

	//Walking North-East
	if (posY > 0.0f && posX > 0.0f)
	{
		totalPos = posX + posY;
		position += forwardVector * totalPos * deltaTime * 20.0f;
		resultVector = DirectX::XMVector3Dot(cameraForward, rightVector);

		if (resultVector.x > -0.6f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, northEastVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.3f);
		}
		else if (resultVector.x < -0.8f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, northEastVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.3f);
		}

		this->rotate();
		return true;
	}

	//Walking North-West
	else if (posY > 0.0f && posX < 0.0f)
	{
		totalPos = abs(posX) + posY;
		position += forwardVector * totalPos * deltaTime * 20.0f;
		resultVector = DirectX::XMVector3Dot(cameraForward, rightVector);

		if (resultVector.x < 0.6f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, northWestVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.3f);
		}
		else if (resultVector.x > 0.8f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, northWestVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.3f);
		}

		this->rotate();
		return true;
	}

	//Walking South-East
	else if (posY < 0.0f && posX > 0.0f)
	{
		totalPos = posX + abs(posY);
		position += forwardVector * totalPos * deltaTime * 20.0f;
		resultVector = DirectX::XMVector3Dot(-cameraForward, rightVector);

		if (resultVector.x < 0.6f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, southEastVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.3f);
		}
		else if (resultVector.x > 0.8f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, southEastVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.3f);
		}

		this->rotate();
		return true;
	}

	//Walking South-West
	else if (posY < 0.0f && posX < 0.0f)
	{
		totalPos = abs(posX) + abs(posY);
		position += forwardVector * totalPos * deltaTime * 20.0f;
		resultVector = DirectX::XMVector3Dot(-cameraForward, rightVector);

		if (resultVector.x > -0.6f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, southWestVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.3f);
		}
		else if (resultVector.x < -0.8f)
		{
			resultVector = DirectX::XMVector3AngleBetweenNormalsEst(forwardVector, southWestVector);
			rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.3f);
		}

		this->rotate();
		return true;
	}

	return false;
}

void Player::moveController(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, const std::unique_ptr<DirectX::GamePad>& gamePad, float deltaTime)
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
			deltaTime *= 2.f;
		}

		//Controller movement
		if (moveCrossController(cameraForward, deltaTime)) {}

		//Walk forward
		else if (posY > 0.0f)
		{
			resultVector = DirectX::XMVector3Dot(cameraForward, rightVector);
			position += forwardVector * posY * deltaTime * 25.0f;

			if (resultVector.x < -0.05f)
			{
				resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x * 0.5f);
			}
			else if (resultVector.x > 0.05f)
			{
				resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
				rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x * 0.5f);
			}
			else
			{
				resultVector = DirectX::XMVector3AngleBetweenNormalsEst(cameraForward, forwardVector);
				if (resultVector.x > XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.5f);
			}

			this->rotate();
		}

		//Walk backward
		else if (posY < 0.0f)
		{
			resultVector = DirectX::XMVector3Dot(-cameraForward, rightVector);
			position += forwardVector * posY * deltaTime * -25.0f;

			if (resultVector.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
			else if (resultVector.x > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
			else
			{
				//Checking where it is
				resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraForward, forwardVector);
				if (resultVector.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}

			this->rotate();
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

			this->rotate();
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

			this->rotate();
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
	if (Input::KeyDown(KeyCode::ENTER))
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

void Player::hitByBat(const reactphysics3d::Vector3& force)
{
	this->dedge = true;
	this->physComp->applyForceToCenter(force);
	this->physComp->applyWorldTorque(force);
	timer.resetStartTime();

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

bool Player::checkForStaticCollision(const std::vector<GameObject*>& gameObjects)
{
	SimpleMath::Vector3 vecPoint = this->position;
	vecPoint += 1.f * forwardVector;
	reactphysics3d::Vector3 point(vecPoint.x, vecPoint.y, vecPoint.z);

	int gameObjSize = (int)gameObjects.size();
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
	int gameObjSize = (int)gameObjects.size();
	for (int i = 0; i < gameObjSize; i++)
	{
		int id = gameObjects.at(i)->getId();
		if ( gameObjects[i]->getPhysComp()->raycast(ray, rayInfo))
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
            DirectX::XMStoreFloat3(&temp, (this->forwardVector*5+ this->getUpDirection()));
            newNormalizeXMFLOAT3(temp);

            //Apply the force
            itemPhysComp->applyLocalTorque(reactphysics3d::Vector3(temp.x * 500, temp.y * 500, temp.z * 500));
            itemPhysComp->applyForceToCenter(reactphysics3d::Vector3(temp.x * 1000, temp.y * 1000, temp.z * 1000));
			holdingItem->setPickedUp(false);

            //You no longer "own" the item
            //holdingItem = nullptr;
        }
        //Use the Item
        else if (Input::KeyDown(KeyCode::T) && Input::KeyDown(KeyCode::T))
        {
            itemPhysComp->setType(reactphysics3d::BodyType::DYNAMIC);
            holdingItem->useItem();
            itemPhysComp->setIsAllowedToSleep(true);
            itemPhysComp->setIsSleeping(true);
			holdingItem->setPickedUp(false);
            //holdingItem = nullptr;
        }
    }
	return dedge;
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
	if (this->particles != nullptr/* && moveKeyPressed*/)
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

DirectX::XMMATRIX Player::getRotationMX()
{
	return this->rotationMX;
}

reactphysics3d::Vector3 Player::getRayCastPos() const
{
	static SimpleMath::Vector3 returnValue;
	returnValue = this->position;
	returnValue += this->normalVector * -1.5f;
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
		if (timer.getTimePassed(5.f)) 
		{ 
			dedge = false; 
			this->physComp->resetForce();
			this->physComp->resetTorque();
			this->physComp->setType(reactphysics3d::BodyType::STATIC); 
			this->position = SimpleMath::Vector3(0, 60, 0);
			this->resetRotationMatrix();
			this->physComp->setPosition(reactphysics3d::Vector3({ this->position.x, this->position.y, this->position.z }));
			this->physComp->setType(reactphysics3d::BodyType::KINEMATIC);
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
		DirectX::XMFLOAT3 rot = this->getRotOrientedToGrav();
		this->particles->setPosition(this->position);
		this->particles->setRotation(this->getUpDirection());
		this->particles->updateBuffer();
	}
}