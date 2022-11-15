#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Player.h"
#include "DirectXMathHelper.h"
#include "BaseballBat.h"
#include "Component.h"
#include "PacketEnum.h"
#include "HudUI.h"
#include "Mesh.h"
#include "SpaceShip.h"
using namespace DirectX;

void Player::throwItem()
{
	//allocates data to be sent
	ComponentData c;
	c.ComponentId = this->getItemOnlineId();
	std::cout << "Item: " << c.ComponentId << "\n";
	c.inUseBy = -1;
	c.packetId = PacketType::COMPONENTPOSITION;
	c.x = this->getPos().x;
	c.y = this->getPos().y;
	c.z = this->getPos().z;
	//sending data to server
	client->sendStuff<ComponentData>(c);

	//Calculate the force vector
	DirectX::XMFLOAT3 temp;
	DirectX::XMStoreFloat3(&temp, (this->forwardVector * 5.f + this->normalVector * 0.5f));
	newNormalizeXMFLOAT3(temp);
	if (this->moveKeyPressed)
	{
		if (this->currentSpeed == this->speed) scalarMultiplicationXMFLOAT3(this->currentSpeed * 0.095f, temp);
		else scalarMultiplicationXMFLOAT3(this->currentSpeed * 0.085f, temp);
	}


	//Set dynamic so it can be affected by forces
	this->holdingItem->getPhysComp()->setType(reactphysics3d::BodyType::DYNAMIC);
	//Apply the force
	this->holdingItem->getPhysComp()->applyForceToCenter(reactphysics3d::Vector3(temp.x * FORCE, temp.y * FORCE, temp.z * FORCE));

	//You no longer "own" the item
	holdingItem->setPickedUp(false);
	holdingItem = nullptr;
}

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

	//Throw the Item
	if (Input::KeyDown(KeyCode::R) && Input::KeyDown(KeyCode::R))
	{
		this->throwItem();
	}
	//Use the Item
	else if (Input::KeyDown(KeyCode::T) && Input::KeyDown(KeyCode::T))
	{
		//allocates data to be sent
		ComponentData c;
		c.ComponentId = this->getItemOnlineId();
		c.inUseBy = -1;
		c.packetId = PacketType::COMPONENTPOSITION;
		c.x = this->getPos().x;
		c.y = this->getPos().y;
		c.z = this->getPos().z;

		//sending data to server
		if (this->client != nullptr)
		{
			client->sendStuff<ComponentData>(c);
		}	

		itemPhysComp->setType(reactphysics3d::BodyType::DYNAMIC);
		holdingItem->useItem();
		itemPhysComp->setIsAllowedToSleep(true);
		itemPhysComp->setIsSleeping(true);
		holdingItem->setPickedUp(false);
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

Player::Player(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, Client* client, const int& team, GravityField* field)
	:GameObject(useMesh, pos, rot, id, field), holdingItem(nullptr), team(team), onlineID(0), currentSpeed(0)
{
	pickUpSfx.load(L"../Sounds/pickupCoin.wav");

	this->rotationMX = XMMatrixIdentity();
	this->rotation = XMMatrixIdentity();
	resultVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	angleVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	this->client = client;
	DirectX::XMStoreFloat4x4(&rotationFloat, this->rotationMX);
	HudUI::player = this;
	
	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(1, 3), 1, true);

	//Item Icon
	float constant = 7.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> playernames{ "Team1_r.png", "Team2_b.png", "player3.png", "player4.png" };
	this->playerIcon = new BilboardObject(playernames, iconPos);
	this->playerIcon->setOffset(constant);

	//Team switch
	switch (team)
	{
	case 0:
		mesh->matKey[0] = "pintoRed.png"; break;
	case 1:
		break;
	}
}

Player::Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, Client* client, const int& team, GravityField* field)
	:GameObject(objectPath, pos, rot, id, field), holdingItem(nullptr), team(team), onlineID(0), speed(25.f), currentSpeed(0)
{
	this->client = client;
	this->rotationMX = XMMatrixIdentity();
	this->rotation = XMMatrixIdentity();
	resultVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	angleVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&rotationFloat, this->rotationMX);

	HudUI::player = this;

	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(1, 3), 1, true);

	//Item Icon
	float constant = 7.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> playernames{ "Team1_r.png", "Team2_b.png", "player3.png", "player4.png" };
	this->playerIcon = new BilboardObject(playernames, iconPos);
	this->playerIcon->setOffset(constant);

	//Team switch
	switch (team)
	{
	case 0:
		mesh->matKey[0] = "pintoRed.png"; break;
	case 1:
		mesh->matKey[0] = "pintoBlue.png"; break;
	}
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
		position += forwardVector * deltaTime * this->currentSpeed;
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
		return true;
	}

	//Walk North-West
	else if (Input::KeyDown(KeyCode::W) && Input::KeyDown(KeyCode::A))
	{
		position += forwardVector * deltaTime * this->currentSpeed;
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
		return true;
	}

	//Walk South-East
	else if (Input::KeyDown(KeyCode::S) && Input::KeyDown(KeyCode::D))
	{
		position += forwardVector * deltaTime * this->currentSpeed;
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
		return true;
	}

	//Walk South-West
	else if (Input::KeyDown(KeyCode::S) && Input::KeyDown(KeyCode::A))
	{
		position += forwardVector * deltaTime * this->currentSpeed;
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
		return true;
	}

	return false;
}

void Player::rotate(const DirectX::XMFLOAT3& grav, const bool& testingVec, const bool& changedPlanet)
{
	if (dedge) return;
	else if (!testingVec) normalVector = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 1.0f);
	else normalVector = DirectX::XMVectorSet(grav.x, grav.y, grav.z, 1.0f);

	//Player jumping to another planet
	if (changedPlanet) flipping = true;
	else if (onGround) flipping = false;

	//Calculations
	rightVector = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	forwardVector = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	normalVector = DirectX::XMVector3Normalize(normalVector);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	//X-Rotation
	resultVector = DirectX::XMVector3Dot(normalVector, forwardVector);
	if (resultVector.x < 0.f)
	{
		resultVector = DirectX::XMVector3Cross(rightVector, normalVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(forwardVector, resultVector);

		//Changing planet, rotating slow
		if (flipping)
		{
			angle = resultVector.x * 0.02f;
			rotation *= DirectX::XMMatrixRotationAxis(rightVector, -angle);
			rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, -angle);
		}
		//Rotating fast like normal
		else
		{
			rotation *= DirectX::XMMatrixRotationAxis(rightVector, -resultVector.x);
			rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, -resultVector.x);
		}
	}
	else if (resultVector.x > 0.f)
	{
		resultVector = DirectX::XMVector3Cross(rightVector, normalVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(forwardVector, resultVector);

		//Changing planet, rotating slow
		if (flipping)
		{
			angle = resultVector.x * 0.02f;
			rotation *= DirectX::XMMatrixRotationAxis(rightVector, angle);
			rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, angle);
		}
		//Rotating fast like normal
		else
		{
			rotation *= DirectX::XMMatrixRotationAxis(rightVector, resultVector.x);
			rotationMX *= DirectX::XMMatrixRotationAxis(rightVector, resultVector.x);
		}
	}

	//Updating vectors
	rightVector = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	forwardVector = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	//Z-Rotation
	resultVector = DirectX::XMVector3Dot(normalVector, rightVector);
	if (resultVector.z < 0.f)
	{
		resultVector = DirectX::XMVector3Cross(normalVector, forwardVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(rightVector, resultVector);

		//Changing planet, rotating slow
		if (flipping)
		{
			angle = resultVector.x * 0.02f;
			rotation *= DirectX::XMMatrixRotationAxis(forwardVector, angle);
			rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, angle);
		}
		//Rotating fast like normal
		else
		{
			rotation *= DirectX::XMMatrixRotationAxis(forwardVector, resultVector.z);
			rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, resultVector.z);
		}
	}
	else if (resultVector.z > 0.f)
	{
		resultVector = DirectX::XMVector3Cross(normalVector, forwardVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(rightVector, resultVector);

		//Changing planet, rotating slow
		if (flipping)
		{
			angle = resultVector.x * 0.02f;
			rotation *= DirectX::XMMatrixRotationAxis(forwardVector, -angle);
			rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, -angle);
		}
		//Rotating fast like normal
		else
		{
			rotation *= DirectX::XMMatrixRotationAxis(forwardVector, -resultVector.z);
			rotationMX *= DirectX::XMMatrixRotationAxis(forwardVector, -resultVector.z);
		}
	}

	//Updating vectors
	rightVector = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	forwardVector = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);
}

void Player::move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const float& deltaTime)
{
	if (dedge) return;
	else if (flipping) return;

	//Running
	this->currentSpeed = this->speed;
	if (Input::KeyDown(KeyCode::SHIFT))
	{
		this->currentSpeed *= 1.5f;
	}

	//Jumping
	if (onGround && Input::KeyPress(KeyCode::SPACE))
	{
		onGround = false;
		this->velocity = this->normalVector * 45.f;
		if (this->moveKeyPressed) this->velocity += this->forwardVector * this->currentSpeed * 0.5f;
	}

	//PC movement
	if (movingCross(cameraForward, deltaTime)) {}

	//Walking forward
	else if (Input::KeyDown(KeyCode::W))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * this->currentSpeed;
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
	}

	//Walking backward
	else if (Input::KeyDown(KeyCode::S))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * this->currentSpeed;
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
	}

	//Walking right
	else if (Input::KeyDown(KeyCode::D))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * this->currentSpeed;
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
	}

	//Walking left
	else if (Input::KeyDown(KeyCode::A))
	{
		this->moveKeyPressed = true;
		position += forwardVector * deltaTime * this->currentSpeed;
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
	}

	//REMOVE AT END!!
	if (Input::KeyDown(KeyCode::I))
	{
		position += cameraForward * deltaTime * this->currentSpeed;
	}

	if (Input::KeyDown(KeyCode::L))
	{
		position += cameraRight * deltaTime * this->currentSpeed;
	}
}

bool Player::moveCrossController(const DirectX::XMVECTOR& cameraForward, float deltaTime) //Need to update if we want to use this
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
			position += forwardVector * posY * deltaTime * speed;

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
		}

		//Walk backward
		else if (posY < 0.0f)
		{
			resultVector = DirectX::XMVector3Dot(-cameraForward, rightVector);
			position += forwardVector * posY * deltaTime * -speed;

			if (resultVector.x < -0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, -0.1f);
			else if (resultVector.x > 0.05f) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.1f);
			else
			{
				//Checking where it is
				resultVector = DirectX::XMVector3AngleBetweenNormalsEst(-cameraForward, forwardVector);
				if (resultVector.x > DirectX::XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, 0.02f);
			}
		}

		//Walk right
		else if (posX > 0.0f)
		{
			position += forwardVector * posX * deltaTime * speed;
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
			position += forwardVector * posX * deltaTime * -speed;
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

	if (!Input::KeyDown(KeyCode::W) && !Input::KeyDown(KeyCode::A) && !Input::KeyDown(KeyCode::S) && !Input::KeyDown(KeyCode::D)) this->moveKeyPressed = false;
}

int Player::getItemOnlineType() const
{
	if (this->holdingItem == nullptr)
	{
		return -1;
	}
	return holdingItem->getOnlineType();
}

int Player::getItemOnlineId() const
{

	return holdingItem->getOnlineId();
}

bool Player::pickupItem(Item* itemToPickup)
{
	bool successfulPickup = false;
	if (Input::KeyDown(KeyCode::E))
	{
		if (!holdingItem && this->withinRadius(itemToPickup, 5))
		{
			addItem(itemToPickup);

			if (itemToPickup->getId() == ObjID::COMPONENT) this->holdingComp = true;
			else this->holdingComp = false;

			holdingItem->getPhysComp()->getRigidBody()->resetForce();
			holdingItem->getPhysComp()->getRigidBody()->resetTorque();
			holdingItem->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
			pickUpSfx.stop();
			pickUpSfx.play();
		}
	}

	return successfulPickup;
}

void Player::setOnlineID(const int& id)
{
	this->onlineID = id;
}

void Player::hitByBat(const reactphysics3d::Vector3& force)
{
	this->physComp->setType(reactphysics3d::BodyType::DYNAMIC);
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

void Player::releaseItem()
{
	if (this->holdingItem != nullptr)
	{
		ComponentData newData;
		newData.packetId = PacketType::COMPONENTPOSITION;
		newData.ComponentId = this->getItemOnlineId();
		newData.inUseBy = -1;
		newData.x = this->holdingItem->getPosV3().x;
		newData.y = this->holdingItem->getPosV3().y;
		newData.z = this->holdingItem->getPosV3().z;
		//sending data to server
		if (this->client != nullptr)
		{
			client->sendStuff<ComponentData>(newData);
		}

		this->holdingItem->setPickedUp(false);
		this->holdingItem->getPhysComp()->setType(reactphysics3d::BodyType::DYNAMIC);
		this->holdingItem = nullptr;
	}
}

bool Player::checkForStaticCollision(const std::vector<Planet*>& gameObjects, const std::vector<SpaceShip*>& spaceShips)
{
	SimpleMath::Vector3 vecPoint = this->position;
	vecPoint += 1.f * forwardVector;
	reactphysics3d::Vector3 point(vecPoint.x, vecPoint.y, vecPoint.z);

	int gameObjSize = (int)gameObjects.size();
	for (int i = 0; i < gameObjSize; i++)
	{
		//if (gameObjects[i]->getPlanetCollider()->getType() != reactphysics3d::BodyType::STATIC || gameObjects[i] == this->holdingItem) continue;
		if (gameObjects[i]->getPlanetCollider()->testPointInside(point))
		{
			this->position -= 1.f * forwardVector;
			return true;
		}
	}
	for (int i = 0; i < spaceShips.size(); i++)
	{
		if (spaceShips[i]->getPhysComp()->testPointInside(point))
		{
			this->position -= 1.f * forwardVector;
			return true;
		}
	}
	return false;
}

bool Player::raycast(const std::vector<GameObject*>& gameObjects, const std::vector<Planet*>& planets, DirectX::XMFLOAT3& hitPos, DirectX::XMFLOAT3& hitNormal)
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
	onGround = false;

	int gameObjSize = (int)gameObjects.size();
	for (int i = 0; i < gameObjSize; i++)
	{
		if (gameObjects[i]->getPhysComp()->getType() != reactphysics3d::BodyType::STATIC) continue;
		if ( gameObjects[i]->getPhysComp()->raycast(ray, rayInfo))
		{
			//Maybe somehow return the index of the triangle hit to calculate new Normal
			hitPos = DirectX::XMFLOAT3(rayInfo.worldPoint.x, rayInfo.worldPoint.y, rayInfo.worldPoint.z);
			hitNormal = DirectX::XMFLOAT3(rayInfo.worldNormal.x, rayInfo.worldNormal.y, rayInfo.worldNormal.z);
			onGround = true;
			return true;
		}
	}
	gameObjSize = (int)planets.size();
	for (int i = 0; i < gameObjSize; i++)
	{
		if (planets[i]->getPlanetCollider()->raycast(ray, rayInfo))
		{
			//Maybe somehow return the index of the triangle hit to calculate new Normal
			hitPos = DirectX::XMFLOAT3(rayInfo.worldPoint.x, rayInfo.worldPoint.y, rayInfo.worldPoint.z);
			hitNormal = DirectX::XMFLOAT3(rayInfo.worldNormal.x, rayInfo.worldNormal.y, rayInfo.worldNormal.z);
			onGround = true;
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

bool Player::getHitByBat() const
{
	if (holdingItem != nullptr)
	{
		DirectX::SimpleMath::Vector3 newPos = this->position;
		newPos += 4 * forwardVector;

		PhysicsComponent* itemPhysComp = holdingItem->getPhysComp();
		holdingItem->setPos(newPos);
		itemPhysComp->setPosition(reactphysics3d::Vector3({ newPos.x, newPos.y, newPos.z }));

		//Throw the Item
		if (Input::KeyDown(KeyCode::R) && Input::KeyDown(KeyCode::R))
		{
			//Set dynamic so it can be affected by forces
			itemPhysComp->setType(reactphysics3d::BodyType::DYNAMIC);

			//Calculate the force vector
			DirectX::XMFLOAT3 temp;
			DirectX::XMStoreFloat3(&temp, (this->forwardVector * 5 + this->getUpDirection()));
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
		}
	}
	return dedge;
}

float Player::getSpeed()const
{
	return this->currentSpeed;
}
void Player::draw()
{
	//Team switch
	switch (team)
	{
	case 0:
		mesh->matKey[0] = "pintoRed.png"; break;
		break;

	case 1:
		mesh->matKey[0] = "pintoBlue.png"; break;
		break;

	}
	//this->mesh->UpdateCB(position, rotation, scale);
	this->mesh->DrawWithMat();
}

void Player::drawIcon()
{
	if (this->playerIcon != nullptr)
	{
		this->playerIcon->bindAndDraw(this->team, 0);
	}
}

void Player::drawParticles()
{
	if (this->particles != nullptr/* && moveKeyPressed*/)
	{
		this->particles->BindAndDraw(0);
	}
}

int Player::getTeam() const
{
	return this->team;
}

DirectX::XMVECTOR Player::getUpVector() const
{
	return this->normalVector;
}

DirectX::XMVECTOR Player::getForwardVector() const
{
	return this->forwardVector;
}

DirectX::XMVECTOR Player::getRightVector() const
{
	return this->rightVector;
}

DirectX::XMMATRIX Player::getRotationMX() const
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

Item* Player::getItem()const
{
	return this->holdingItem;
}

int Player::getOnlineID() const
{
	return this->onlineID;
}

bool Player::isHoldingItem() const
{
	bool isHolding = false;
	if (this->holdingItem == nullptr)
	{
		isHolding = false;
	}
	else
	{
		isHolding = true;
	}
	return isHolding;
}

void Player::setSpeed(float speed)
{
	this->speed = speed;
}

void Player::update()
{
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

void Player::setTeam(const int& team)
{
	this->team = team;

	//Team switch
	switch (team)
	{
	case 0:
		mesh->matKey[0] = "pintoRed.png"; break;
	case 1:
		mesh->matKey[0] = "pintoBlue.png"; break;
	}
}

bool Player::isHoldingComp()
{
	if (holdingComp)
	{
		if (this->holdingItem != nullptr)
		{
			this->setSpeed(25.f * 0.65f);
			return true;
		}
		else
		{
			this->setSpeed(25.f);
			return false;
		}
	}

	return false;
}

void Player::updateVelocity(const DirectX::SimpleMath::Vector3& gravityVector)
{
	this->velocity += gravityVector;
}

void Player::resetVelocity()
{
	this->velocity = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);
}

void Player::velocityMove(const float& dt)
{
	this->position += velocity * dt;
}